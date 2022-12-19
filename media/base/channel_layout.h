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

        // Front L, Front R, Front C
        CHANNEL_LAYOUT_SURROUND = 5,

        // Front L, Front R, Front C, Back C
        CHANNEL_LAYOUT_4_0 = 6,

        // Front L, Front R, Side L, Side R
        CHANNEL_LAYOUT_2_2 = 7,

        // Front L, Front R, Back L, Back R
        CHANNEL_LAYOUT_QUAD = 8,

        // Front L, Front R, Front C, Side L, Side R
        CHANNEL_LAYOUT_5_0 = 9,

        // Front L, Front R, Front C, LFE, Side L, Side R
        CHANNEL_LAYOUT_5_1 = 10,

        // Front L, Front R, Front C, Back L, Back R
        CHANNEL_LAYOUT_5_0_BACK = 11,

        // Front L, Front R, Front C, LFE, Back L, Back R
        CHANNEL_LAYOUT_5_1_BACK = 12,

        // Front L, Front R, Front C, Side L, Side R, Back L, Back R
        CHANNEL_LAYOUT_7_0 = 13,

        // Front L, Front R, Front C, LFE, Side L, Side R, Back L, Back R
        CHANNEL_LAYOUT_7_1 = 14,

        // Front L, Front R, Front C, LFE, Side L, Side R, Front LofC, Front RofC
        CHANNEL_LAYOUT_7_1_WIDE = 15,

        // Max value, must always equal the largest entry ever logged.
        CHANNEL_LAYOUT_MAX = CHANNEL_LAYOUT_7_1_WIDE
    };

    // Note: Do not reorder or reassign these values.
    enum Channels {
        LEFT = 0,
        RIGHT,
        CENTER,
        LFE,        // Low Frequency Effects
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

    // Return the number of channels in a given ChannelLayout or 0 if the
    // channel layout can't be mapped to a valid value. Currently, 0
    // is returned for CHANNEL_LAYOUT_NONE, CHANNEL_LAYOUT_UNSUPPORTED。
    // For these cases, additional steps must be taken to manually figure out
    // the corresponding number of channels.
    int ChannelLayoutToChannelCount(ChannelLayout layout);

    // Given the number of channels, return the base layout,
    // or return CHANNEL_LAYOUT_UNSUPPORTED if there is no good match.
    ChannelLayout GuessChannelLayout(int channels);

    // Return a string representation of the channel layout.
    const char* ChannelLayoutToString(ChannelLayout layout);
}

#endif //NEWCUT_CHANNEL_LAYOUT_H
