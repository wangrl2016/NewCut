//
// Created by wangrl2016 on 2022/12/20.
//

#include <glog/logging.h>
#include "media/base/channel_layout.h"
#include "media/base/audio_parameters.h"

namespace media {
    ChannelLayoutConfig::ChannelLayoutConfig() :
            ChannelLayoutConfig(
                    ChannelLayoutConfig::FromLayout<CHANNEL_LAYOUT_NONE>()) {

    }

    ChannelLayoutConfig::ChannelLayoutConfig(ChannelLayout channel_layout, int channels)
            : channel_layout_(channel_layout), channels_(ChannelLayoutToChannelCount(channel_layout)) {
        DCHECK(channel_layout == CHANNEL_LAYOUT_UNSUPPORTED ||
               channels_ == channels);
    }

    ChannelLayoutConfig ChannelLayoutConfig::Mono() {
        return FromLayout<CHANNEL_LAYOUT_MONO>();
    }

    ChannelLayoutConfig ChannelLayoutConfig::Stereo() {
        return FromLayout<CHANNEL_LAYOUT_STEREO>();
    }

    ChannelLayoutConfig ChannelLayoutConfig::Guess(int channels) {
        return {GuessChannelLayout(channels), channels};
    }

    AudioParameters::AudioParameters()
        : AudioParameters(AUDIO_PCM_LINEAR,
                          ChannelLayoutConfig::FromLayout<CHANNEL_LAYOUT_NONE>(),
                                  0,
                                  0) {}

    AudioParameters::AudioParameters(Format format,
                                     ChannelLayoutConfig channel_layout_config,
                                     int sample_rate,
                                     int frames_per_buffer) {
        Reset(format, channel_layout_config, sample_rate, frames_per_buffer);
    }

    AudioParameters::~AudioParameters() = default;

    void AudioParameters::Reset(Format format,
                                ChannelLayoutConfig channel_layout_config,
                                int sample_rate,
                                int frames_per_buffer) {
        format_ = format;
        channel_layout_config_ = channel_layout_config;
        sample_rate_ = sample_rate;
        frames_per_buffer_ = frames_per_buffer;
    }
}
