//
// Created by wangrl2016 on 2022/12/17.
//

#include "media/base/audio_bus.h"
#include "media/base/limits.h"

namespace media {
    static bool IsAligned(void* ptr) {
        return (reinterpret_cast<uintptr_t>(ptr) &
                (AudioBus::kChannelAlignment - 1)) == 0U;
    }

    // In order to guarantee that the memory block for each channel starts at an
    // aligned address when splitting a contiguous block of memory into one block
    // per channel, we may have to make these blocks larger than otherwise needed.
    // We do this by allocating space for potentially more frames than requested.
    // This method returns the required size for the contiguous memory block
    // in bytes and outputs the adjusted number of frames via |out_aligned_frames).
    static int CalculateMemorySizeInternal(int channels,
                                           int frames,
                                           int* out_aligned_frames) {
        // Since our internal sample format is float, we can guarantee the alignment
        // by making the number of frames an integer multiple of
        // AudioBus::kChannelAlignment / sizeof(float).
        int aligned_frames = ((frames * sizeof(float) + AudioBus::kChannelAlignment - 1) &
                              ~(AudioBus::kChannelAlignment - 1)) / sizeof(float);

        if (out_aligned_frames) {
            *out_aligned_frames = aligned_frames;
        }
        return sizeof(float) * channels * aligned_frames;
    }

    static void ValidateConfig(int channels, int frames) {
        CHECK_GT(frames, 0);
        CHECK_GT(channels, 0);
        CHECK_LE(channels, static_cast<int>(kMaxChannels));
    }
}
