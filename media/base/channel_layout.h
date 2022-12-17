//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_CHANNEL_LAYOUT_H
#define NEWCUT_CHANNEL_LAYOUT_H

namespace media {
    // Enumerates the various representations of the ordering of audio channels.
    // Logged to UMA, so never reuse a value, always add new/greater ones!
    enum ChannelLayout {
        CHANNEL_LAYOUT_NONE = 0,
        CHANNEL_LAYOUT_UNSUPPORTED = 1,

        // Front C
        CHANNEL_LAYOUT_MONO = 2,

        // Front L, Front R
        CHANNEL_LAYOUT_STEREO = 3,

        // Front L, Front R, Back C
        CHANNEL_LAYOUT_2_1 = 4,

        // Front L, Front R, Front C, Back C
        CHANNEL_LAYOUT_SURROUND = 5,

        // Front L, Front R, Front C, Back C
        CHANNEL_LAYOUT_4_0 = 6,

        // Front L, Front R, Side L, Side R
        CHANNEL_LAYOUT_2_2 = 7,

        // Front L, Front R, Back L, Back R
        CHANNEL_LAYOUT_QUAD = 8,
    };

    enum Channels {
        LEFT = 0,
        RIGHT,
        CENTER,
        LEF,
        BACK_LEFT,
        BACK_RIGHT,
        LEFT_OF_CENTER,
        RIGHT_OF_CENTER,
        BACK_CENTER,
        SIDE_LEFT,
        SIDE_RIGHT,
        CHANNELS_MAX = SIDE_RIGHT, // must always equals the largest value ever logged
    };

    constexpr int kMaxConcurrentChannels = 8;

    // Returns the expected channel position in an interleaved stream. Values of -1
    // mean the channel at that index is not used for that layout. Values range
    // from 0 to ChannelLayoutToChannelCount(layout) - 1.
    int ChannelOrder(ChannelLayout layout, Channels channels);

    int ChannelLayoutToChannelCount(ChannelLayout layout);
}


#endif //NEWCUT_CHANNEL_LAYOUT_H
