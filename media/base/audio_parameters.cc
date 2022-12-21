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
}
