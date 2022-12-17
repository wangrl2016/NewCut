//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_AUDIO_BUS_H
#define NEWCUT_AUDIO_BUS_H

#include <memory>

namespace media {
    class AudioParameters;

    // Represents a sequence of audio frames containing frames() audio samples for
    // each of channels() channels. The data is stored as a set of contiguous
    // float arrays with one array per channel. The memory for teh arrays is either
    // allocated and owned by the AudioBus or it is provided to one of the factory
    // methods. AudioBus guarantees that it allocates memory such that float array
    // for each channel is aligned by AudioBus::kChannelAlignment bytes, and it
    // requires the same for memory passed to its Wrap...() factory methods.
    class AudioBus {
    public:
        // Guaranteed alignment of each channel's data; use 16-byte alignment for easy
        // SSE optimizations.
        enum {
            kChannelAlignment = 16
        };

        // Creates a new AudioBus and allocates |channels| of length |frames|. Uses
        // channels() and frames_per_buffer() from AudioParameters if given.
        static std::unique_ptr<AudioBus> Create(int channels, int frames);

        static std::unique_ptr<AudioBus> Create(const AudioParameters& params);

        // Creates a new AudioBus with the given number of channels, but zero length.
        // Clients are expected to subsequently call SetChannelData() and set_frames()
        // to wrap externally allocated memory.
        static std::unique_ptr<AudioBus> CreateWrapper(int channels);

        // Create a new AudioBus from an existing channel vector. Does not transfer
        // ownership of |channel_data| to AudioBus; i.e., |channel_data| must outlive
        // the returned AudioBus. Each channel must be aligned by kChannelAlignment.
        static std::unique_ptr<AudioBus> WrapVector(
                int frames,
                const std::vector<float*>& channel_data);

        // Creates a new AudioBus by wrapping an exising block of memory. Block must
        // be at least CalculateMemorySize() bytes in size. |data| must outlive the
        // returned AudioBus. |data| must be aligned by kChannelAlignment.
        static std::unique_ptr<AudioBus> WrapMemory(int channels,
                                                    int frames,
                                                    void* data);

        static std::unique_ptr<AudioBus> WrapMemory(const AudioParameters& params,
                                                    void* data);

        static std::unique_ptr<const AudioBus> WrapReadOnlyMemory(int channels,
                                                                  int frames,
                                                                  const void* data);

        static std::unique_ptr<const AudioBus> WrapReadOnlyMemory(
                const AudioParameters& params,
                const void* data);
    };
}


#endif //NEWCUT_AUDIO_BUS_H
