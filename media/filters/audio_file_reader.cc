//
// Created by wangrl2016 on 2022/12/17.
//

#include <glog/logging.h>
#include "media/ffmpeg/ffmpeg_common.h"
#include "media/filters/audio_file_reader.h"

namespace media {
    AudioFileReader::AudioFileReader(media::FFmpegURLProtocol* protocol)
            : stream_index_(0),
              protocol_(protocol),
              audio_codec_(AudioCodec::kUnknown),
              channels_(0),
              sample_rate_(0),
              av_sample_format_(0) {}

    AudioFileReader::~AudioFileReader() {
        Close();
    }

    bool AudioFileReader::Open() {
        return OpenDemuxer() && OpenDecoder();
    }

    bool AudioFileReader::OpenDemuxer() {
        glue_ = std::make_unique<FFmpegGlue>(protocol_);
        auto* format_context = glue_->format_context();

        // Open FFmpeg AVFormatContext.
        if (!glue_->OpenContext()) {
            LOG(WARNING) << __FUNCTION__ << ": error in avformat_open_input()";
            return false;
        }

        const int result = avformat_find_stream_info(format_context, nullptr);
        if (result < 0) {
            DLOG(WARNING) << __FUNCTION__ << ": error in avformat_find_stream_info()";
            return false;
        }

        // Calling avformat_find_stream_info can uncover new streams. We wait till now
        // to find the first audio stream, if any.
        codec_context_.reset();
        bool found_stream = false;
        for (size_t i = 0; i < format_context->nb_streams; i++) {
            if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                stream_index_ = i;
                found_stream = true;
                break;
            }
        }

        if (!found_stream) {
            return false;
        }

        // Get the codec context.
        codec_context_ = AVStreamToAVCodecContext(format_context->streams[stream_index_]);
        if (!codec_context_) {
            return false;
        }
        DCHECK_EQ(codec_context_->codec_type, AVMEDIA_TYPE_AUDIO);
        return true;
    }

    bool AudioFileReader::OpenDecoder() {
        const AVCodec* codec = avcodec_find_decoder(codec_context_->codec_id);
        if (codec) {
            // MP3 decodes to S16P which we don't support, tell it to use S16 instead.
            if (codec_context_->sample_fmt == AV_SAMPLE_FMT_S16P) {
                codec_context_->request_sample_fmt = AV_SAMPLE_FMT_S16;
            }
            const int result = avcodec_open2(codec_context_.get(), codec, nullptr);
            if (result < 0) {
                DLOG(WARNING) << __FUNCTION__ << ": could not open codec - result " << result;
                return false;
            }

            // Ensure avcodec_open2() respected our format request.
            if (codec_context_->sample_fmt == AV_SAMPLE_FMT_S16P) {
                DLOG(ERROR) << __FUNCTION__ << ": unable to configure a supported sample format - "
                            << codec_context_->sample_fmt;
                return false;
            }
        } else {
            DLOG(WARNING) << __FUNCTION__ << ": could not find codec.";
            return false;
        }

        // Store initial values to guard against midstream configuration changes.
        channels_ = codec_context_->channels;
        audio_codec_ = CodecIDToAudioCodec(codec_context_->codec_id);
        sample_rate_ = codec_context_->sample_rate;
        av_sample_format_ = codec_context_->sample_fmt;
        return true;
    }

    bool AudioFileReader::HasKnownDuration() const {
        return glue_->format_context()->duration != AV_NOPTS_VALUE;
    }

    void AudioFileReader::Close() {
        codec_context_.reset();
        glue_.reset();
    }
}