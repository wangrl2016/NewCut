//
// Created by wangrl2016 on 2022/12/17.
//

#include <glog/logging.h>
#include <cmath>
#include "media/ffmpeg/ffmpeg_common.h"
#include "media/filters/audio_file_reader.h"

namespace media {
    // AAC(M4A) decoding specific constants.
    static const int kAACPrimingFrameCount = 2112;
    static const int kAACRemainerFrameCount = 519;

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

    void AudioFileReader::Close() {
        codec_context_.reset();
        glue_.reset();
    }

    int AudioFileReader::Read(
            std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
            int packets_to_read) {
        DCHECK(glue_ && codec_context_) << __FUNCTION__ << ": reader is not opened!";

    }

    bool AudioFileReader::HasKnownDuration() const {
        return glue_->format_context()->duration != AV_NOPTS_VALUE;
    }

    int64_t AudioFileReader::GetDuration() const {
        const AVRational av_time_base = {1, AV_TIME_BASE};
        DCHECK_NE(glue_->format_context()->duration, AV_NOPTS_VALUE);
        int64_t estimated_duration_us = glue_->format_context()->duration;

        if (audio_codec_ == AudioCodec::kAAC) {
            // For certain AAC-encoded files, FFMPEG's estimated frame count might not
            // be sufficient to capture the entire audio content that we want. This is
            // especially noticeable for short files (< 10ms) resulting in silence
            // thourghout the decoded buffer. Thus, we add the priming frames and the
            // remainder frames to the estimation.
            estimated_duration_us += ceil(
                    1000000.0 *
                    static_cast<double>(kAACPrimingFrameCount + kAACRemainerFrameCount) /
                    sample_rate());
        } else {
            // Add one microsecond to avoid rounding-down erros which can occur when
            // |duration| has been calculated from an exact number of sample-frames.
            // One microsecond is much less than the time of a single sample-frame
            // at any real-world sample rate.
            estimated_duration_us += 1;
        }

        return ConvertFromTimeBase(av_time_base, estimated_duration_us);
    }

    int AudioFileReader::GetNumberOfFrames() const {
        return int(std::round(static_cast<double>(GetDuration()) /
                              base::Time::kMicrosecondPerSecond * sample_rate_));
    }

    bool AudioFileReader::OpenDemuxerForTesting() {
        return OpenDemuxer();
    }

    bool AudioFileReader::ReadPacketForTesting(AVPacket* output_packet) {
        return ReadPacket(output_packet);
    }

    bool AudioFileReader::SeekForTesting(int64_t seek_time) {
        return av_seek_frame(
                glue_->format_context(), stream_index_,
                // 转化为FFmpeg表示的时间
                ConvertToTimeBase(GetAVStreamForTesting()->time_base, seek_time),
                AVSEEK_FLAG_BACKWARD) >= 0;
    }

    const AVStream* AudioFileReader::GetAVStreamForTesting() const {
        return glue_->format_context()->streams[stream_index_];
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

    bool AudioFileReader::ReadPacket(AVPacket* output_packet) {
        while (av_read_frame(glue_->format_context(), output_packet) >= 0) {
            // Skip packets from other streams.
            if (output_packet->stream_index != stream_index_) {
                av_packet_unref(output_packet);
                continue;
            }
            return true;
        }
        return false;
    }

    bool AudioFileReader::OnNewFrame(
            int* total_frames,
            std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
            AVFrame* frame) {
        int frames_read = frame->nb_samples;
        if (frames_read < 0)
            return false;

        const int channels = frame->channels;
        if (frame->sample_rate != sample_rate_ || channels != channels_ ||
            frame->format != av_sample_format_) {
            DLOG(ERROR) << "Unsupported midstream configuration change!"
                        << " Sample Rate: " << frame->sample_rate << " vs "
                        << sample_rate_ << ", Channels: " << channels << " vs "
                        << channels_ << ", Sample Format: " << frame->format << " vs "
                        << av_sample_format_;

            // This is an unrecoverable error, so bail out. We'll return
            // whatever we've decoded up to this point.
            return false;
        }

        // AAC decoding doesn't properly trim the last packet in a stream, so if we
        // have duration information, use it to set the correct length to avoid extra
        // silence from being output. In the case where we are also discarding some
        // portion of the packet (as indicated by a negative pts), we further want to
        // adjust the duration downward by however much exists before zero.
        if (audio_codec_ == AudioCodec::kAAC && frame->pkt_duration) {
            const int64_t pkt_duration = ConvertFromTimeBase(
                    glue_->format_context()->streams[stream_index_]->time_base,
                    frame->pkt_duration + std::min(static_cast<int64_t>(0), frame->pts));

            const auto frame_duration = int64_t((frames_read / static_cast<double>(sample_rate_)) *
                    base::Time::kMicrosecondPerSecond);

            if (pkt_duration < frame_duration && pkt_duration > 0) {
                const int new_frames_read =
                        int(frames_read * (pkt_duration / frame_duration));
                DVLOG(google::WARNING) << "Shrinking AAC frame from " << frames_read << " to "
                        << new_frames_read << " based on packet duration.";
                frames_read = new_frames_read;
            }

            // The above process may delete the entire packet.
            if (!frames_read)
                return true;
        }

        // Deinterleave each channel and convert to 32bit floating-point with
        // nominal range -1.0 -> +1.0. If the output is already in float planar
        // format, just copy it into the AudioBus.
        decoded_audio_packets->emplace_back(AudioBus::Create(channels, frames_read));
        auto* audio_bus = decoded_audio_packets->back().get();

        if (codec_context_->sample_fmt == AV_SAMPLE_FMT_FLT) {

        }
    }
}
