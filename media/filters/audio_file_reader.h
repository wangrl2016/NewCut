//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_AUDIO_FILE_READER_H
#define NEWCUT_AUDIO_FILE_READER_H

#include <vector>
#include <memory>
#include <libavcodec/avcodec.h>
#include "media/filters/ffmpeg_glue.h"
#include "media/base/audio_codec.h"

namespace media {
    class AudioBus;

    class AudioFileReader {
    public:
        // Audio file data will be read using the given protocol.
        // The AudioFileReader does not take ownership of |protocol| and
        // simply maintains a weak reference to it.
        explicit AudioFileReader(FFmpegURLProtocol* protocol);

        AudioFileReader(const AudioFileReader&) = delete;

        AudioFileReader& operator=(const AudioFileReader&) = delete;

        virtual ~AudioFileReader();

        // Open() reads the audio data format so that the sample_rate(),
        // channels(), GetDuration(), and GetNumberOfFrames() methods can be called.
        // It returns |true| on success.
        bool Open();

        void Close();

        // After a call to open(), attemps to decode the data of |packets_to_read|,
        // updating |decodedAudioPackets| with each decoded packet in order.
        // The caller must convert these packets into one complete set of
        // decoded audio data. The audio data will be decoded as
        // floating-point linear PCM with a nominal range of -1.0 -> +1.0.
        // Return the number of sample-frames actually read which will
        // always be the total size of all the frames in
        // |decoded_audio_packets|.
        // If |packets_to_read| is std::numeric_limits<int>::max(), decodes
        // the entire data.
        int Read(std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
                 int packets_to_read = std::numeric_limits<int>::max());

        int channels() const { return channels_; }

        int sample_rate() const { return sample_rate_; }

        // Return true if (an estimated) duration of the audio data is known.
        // Must be called after Open().
        bool HasKnownDuration() const;

        // Please note that GetDuration() and GetNumberOfFrames() attempt to be
        // accurate, but are onlye estimates. For smoe encoded formats, the actual
        // duration of the file can only be determined once all the file data has been
        // read. The Read() method returns the actual number of sample-frames is has
        // read.
        int64_t GetDuration() const;

        int GetNumberOfFrames() const;

        // The method below are helper methods which allow AudioFileReader to double
        // as a test utility for demuxing audio files.

        // Similar to Open() but does not initialize the decoder.
        bool OpenDemuxerForTesting();

        // Return true if a packet could be demuxed from the firat audio stream in
        // the file, |output_packet| will contain the demuxed packet then.
        bool ReadPacketForTesting(AVPacket* output_packet);

        // Seeks to the given point and return true if successful. |seek_time| will
        //be converted to the stream's time base automatically.
        bool SeekForTesting(int64_t seek_time);

        const AVStream* GetAVStreamForTesting() const;

        const AVCodecContext* codec_context_for_testing() const {
            return codec_context_.get();
        }

    private:
        bool OpenDemuxer();

        bool OpenDecoder();

        bool ReadPacket(AVPacket* output_packet);

        bool OnNewFrame(int* total_frames,
                        std::vector<std::unique_ptr<AudioBus>>* decoded_audio_packets,
                        AVFrame* frame);

        // Destruct |glue_| after |codec_context_|.
        std::unique_ptr<FFmpegGlue> glue_;
        std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext> codec_context_;

        int stream_index_;
        FFmpegURLProtocol* protocol_;
        AudioCodec audio_codec_;
        int channels_;
        int sample_rate_;

        // AVSampleFormat initially requested.
        int av_sample_format_;
    };
}

#endif //NEWCUT_AUDIO_FILE_READER_H
