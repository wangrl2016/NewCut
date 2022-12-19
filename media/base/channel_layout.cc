//
// Created by wangrl2016 on 2022/12/17.
//

#include <glog/logging.h>
#include "media/base/channel_layout.h"

namespace media {
    static const int kLayoutToChannels[] = {
            0,  // CHANNEL_LAYOUT_NONE
            0,  // CHANNEL_LAYOUT_UNSUPPORTED
            1,  // CHANNEL_LAYOUT_MONO
            2,  // CHANNEL_LAYOUT_STEREO
            3,  // CHANNEL_LAYOUT_2_1
            3,  // CHANNEL_LAYOUT_SURROUND
            4,  // CHANNEL_LAYOUT_4_0
            4,  // CHANNEL_LAYOUT_2_2
            4,  // CHANNEL_LAYOUT_QUAD
            5,  // CHANNEL_LAYOUT_5_0
            6,  // CHANNEL_LAYOUT_5_1
            5,  // CHANNEL_LAYOUT_5_0_BACK
            6,   // CHANNEL_LAYOUT_5_1_BACK
            7,   // CHANNEL_LAYOUT_7_0
            8,   // CHANNEL_LAYOUT_7_1
            8,   // CHANNEL_LAYOUT_7_1_WIDE
    };

    // The channel orderings for each layout as specified by FFmpeg. Each value
    // represents the index of each channel in each layout. Values of -1 mean the
    // channel at that index is not used for that layout. For example, the left side
    // surround sound channel in FFmpeg 5.1 layout is in the 5th position (because
    // the order is L, R, C, LFE, LS, RS), so
    // kChannelOrdering[CHANNEL_LAYOUT_5_1][SIDE_LEFT] = 4
    static const int kChannelOrdering[CHANNEL_LAYOUT_MAX + 1][CHANNELS_MAX + 1] = {
            // FL | FR | FC | LFE | BL | BR | FLofC | FRofC | BC | SL | SR

            // CHANNEL_LAYOUT_NONE
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_UNSUPPORTED
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_MONO
            {-1, -1, 0,  -1, -1, -1, -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_STEREO
            {0,  1,  -1, -1, -1, -1, -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_2_1
            {0,  1,  -1, -1, -1, -1, -1, -1, 2,  -1, -1},

            // CHANNEL_LAYOUT_SURROUND
            {0,  1,  2,  -1, -1, -1, -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_4_0
            {0,  1,  2,  -1, -1, -1, -1, -1, 3,  -1, -1},

            // CHANNEL_LAYOUT_2_2
            {0,  1,  -1, -1, -1, -1, -1, -1, -1, 2,  3},

            // CHANNEL_LAYOUT_QUAD
            {0,  1,  -1, -1, 2,  3,  -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_5_0
            {0,  1,  2,  -1, -1, -1, -1, -1, -1, 3,  4},

            // CHANNEL_LAYOUT_5_1
            {0,  1,  2,  3,  -1, -1, -1, -1, -1, 4,  5},

            // CHANNEL_LAYOUT_5_0_BACK
            {0,  1,  2,  -1, 3,  4,  -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_5_1_BACK
            {0,  1,  2,  3,  4,  5,  -1, -1, -1, -1, -1},

            // CHANNEL_LAYOUT_7_0
            {0,  1,  2,  -1, 5,  6,  -1, -1, -1, 3,  4},

            // CHANNEL_LAYOUT_7_1
            {0,  1,  2,  3,  6,  7,  -1, -1, -1, 4,  5},

            // CHANNEL_LAYOUT_7_1_WIDE
            {0,  1,  2,  3,  -1, -1, 6,  7,  -1, 4,  5},
    };

    int ChannelOrder(ChannelLayout layout, Channels channel) {
        DCHECK_LT(static_cast<size_t>(layout), std::size(kChannelOrdering));
        DCHECK_LT(static_cast<size_t>(channel), std::size(kChannelOrdering[0]));
        return kChannelOrdering[layout][channel];
    }

    int ChannelLayoutToChannelCount(ChannelLayout layout) {
        DCHECK_LT(static_cast<size_t>(layout), std::size(kLayoutToChannels));
        DCHECK_LE(kLayoutToChannels[layout], kMaxConcurrentChannels);
        return kLayoutToChannels[layout];
    }

    ChannelLayout GuessChannelLayout(int channels) {
        switch (channels) {
            case 1:
                return CHANNEL_LAYOUT_MONO;
            case 2:
                return CHANNEL_LAYOUT_STEREO;
            case 3:
                return CHANNEL_LAYOUT_SURROUND;
            case 4:
                return CHANNEL_LAYOUT_QUAD;
            case 5:
                return CHANNEL_LAYOUT_5_0;
            case 6:
                return CHANNEL_LAYOUT_5_1;
            case 8:
                return CHANNEL_LAYOUT_7_1;
            default:
                LOG(INFO) << "Unsupported channel count: " << channels;
        }
        return CHANNEL_LAYOUT_UNSUPPORTED;
    }

    const char* ChannelLayoutToString(ChannelLayout layout) {
        switch (layout) {
            case CHANNEL_LAYOUT_NONE:
                return "NONE";
            case CHANNEL_LAYOUT_UNSUPPORTED:
                return "UNSUPPORTED";
            case CHANNEL_LAYOUT_MONO:
                return "MONO";
            case CHANNEL_LAYOUT_STEREO:
                return "STEREO";
            case CHANNEL_LAYOUT_2_1:
                return "2.1";
            case CHANNEL_LAYOUT_SURROUND:
                return "SURROUND";
            case CHANNEL_LAYOUT_4_0:
                return "4.0";
            case CHANNEL_LAYOUT_2_2:
                return "QUAD_SIDE";
            case CHANNEL_LAYOUT_QUAD:
                return "QUAD";
            case CHANNEL_LAYOUT_5_0:
                return "5.0";
            case CHANNEL_LAYOUT_5_1:
                return "5.1";
            case CHANNEL_LAYOUT_5_0_BACK:
                return "5.0_BACK";
            case CHANNEL_LAYOUT_5_1_BACK:
                return "5.1_BACK";
            case CHANNEL_LAYOUT_7_0:
                return "7.0";
            case CHANNEL_LAYOUT_7_1:
                return "7.1";
            case CHANNEL_LAYOUT_7_1_WIDE:
                return "7.1_WIDE";
        }
        LOG(INFO) << "Invalid channel layout provided: " << layout;
        return "";
    }
}
