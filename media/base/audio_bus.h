//
// Created by wangrl2016 on 2022/12/17.
//

#ifndef NEWCUT_AUDIO_BUS_H
#define NEWCUT_AUDIO_BUS_H

#include <memory>
#include <vector>
#include "base/memory/aligned_memory.h"

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

        static int CalculateMemorySize(int channels, int frames);

        static int CalculateMemorySize(const AudioParameters& params);

        // Methods that are expectd to be called after AudioBus::CreateWrapper() in
        // order to wrap externally allocated memory. Note: It is illegal to call
        // these methods when using a factory method other than CreateWrapper().
        void SetChannelData(int channel, float* data);

        void set_frames(int frames);

        // Method optionally called after AudioBus::CreateWrapper().
        // Runs |deleter| when on |this| desruction, freeing external data
        // referenced by SetChannelData().
        // Note: It is illegal to call this method when using a factory method oter
        // than CreateWrapper().
        void SetWrappedDataDeleter(void (*deleter)());

        // Methods for compressed bitstream formats. The data size may not be equal to
        // the capacity of the AudioBus. Also, the frame count may not be equal to the
        // capacity of the AudioBus. Thus, we need extra methods to access the real
        // data size and frame count for bitstream formats.
        bool is_bitstram_format() const { return is_bitstream_format_; }

        void set_is_bitstream_format(bool is_bitstream_format) {
            is_bitstream_format_ = is_bitstream_format;
        }

        size_t GetBitstreamDataSize() const;

        void SetBitstreamDataSize(size_t data_size);

        int GetBitstreamFrames() const;

        void SetBitstreamFrames(int frames);
    private:
        // contiguous block of channel memory.
        std::unique_ptr<float, base::AlignedFreeDeleter> data_;

        // Whether the data is compressed bitstream or not.
        bool is_bitstream_format_ = false;
        // The data size for a compressed bitstream.
        size_t bitstream_data_size_ = 0;
        // The PCM frame count for a compressed bitstream.
        int bitstream_frames_ = 0;

        // One float pointer per channel pointing to a contiguous block of memory for
        // that channel. If the memory is owned by this instance, this will
        // point to the memory in |data_|. Otherwise, it may point to memory provided
        // by the client.
        std::vector<float*> channel_data_;
        int frames_;
    };
}


#endif //NEWCUT_AUDIO_BUS_H
