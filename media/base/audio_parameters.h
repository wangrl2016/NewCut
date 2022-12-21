//
// Created by wangrl2016 on 2022/12/20.
//

#ifndef NEWCUT_AUDIO_PARAMETERS_H
#define NEWCUT_AUDIO_PARAMETERS_H

#include "media/base/channel_layout.h"
#include "media/base/sample_format.h"

namespace media {
    // Channel count and ChannelLayout pair, with helper method to enforce safe
    // construction.
    class ChannelLayoutConfig {
    public:
        ChannelLayoutConfig(const ChannelLayoutConfig& other) = default;

        ChannelLayoutConfig& operator=(const ChannelLayoutConfig& other) = default;

        ChannelLayoutConfig();

        ChannelLayoutConfig(ChannelLayout channel_layout, int channels);

        ~ChannelLayoutConfig() = default;

        // 静态模版函数
        template<ChannelLayout layout>
        static ChannelLayoutConfig FromLayout() {
            return {layout, ChannelLayoutToChannelCount(layout)};
        }

        static ChannelLayoutConfig Mono();

        static ChannelLayoutConfig Stereo();

        static ChannelLayoutConfig Guess(int channels);

        ChannelLayout channel_layout() const {
            return channel_layout_;
        }

        int channels() const {
            return channels_;
        }

    private:
        ChannelLayout channel_layout_;      // Order of surround sound channels.
        int channels_;                      // Number of channels.
    };

    class AudioParameters {
    public:
        enum Format {
            AUDIO_FAKE = 0x000,             // Create a fake AudioOutputStream object.
            AUDIO_PCM_LINEAR = 0x001,       // PCM is 'raw' amplitude samples.
            AUDIO_PCM_LOW_LATENCY = 0x002,  // Linear PCM, low latency requested.
            AUDIO_BITSTREAM_AC3 = 0x004,    // Compressed ACe bitstream.
            AUDIO_FORMAT_LAST =
            AUDIO_BITSTREAM_AC3,    // Only used for validation of format.
        };

        AudioParameters();

        AudioParameters(Format format,
                        ChannelLayoutConfig channel_layout_config,
                        int sample_rate,
                        int frames_per_buffer);

        ~AudioParameters();

        void Reset(Format format,
                   ChannelLayoutConfig channel_layout_config,
                   int sample_rate,
                   int frames_per_buffer);



    private:
        Format format_;                                 // Format of the stream
        ChannelLayoutConfig channel_layout_config_;     // The channel layout and the number of channels.
        int sample_rate_;                               // Sampling frequency/rate.
        int frames_per_buffer_;                         // Number of frames in buffer.
    };
}


#endif //NEWCUT_AUDIO_PARAMETERS_H
