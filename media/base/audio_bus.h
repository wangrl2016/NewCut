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
        void SetWrappedDataDeleter(void (* deleter)());

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

        // Overwrites the sample values stored in this AudioBus instance with values
        // from a given interleaved |source_buffer| with expect layout
        // [ch0, ch1, ..., chN, ch0, ch1, ...] and sample values in the format
        // corresponding to the given SourceSampleTypeTraits.
        // The sample values are converted to float values by means of the method
        // convert_to_float32() provided by the SourceSampleTypeTraits. For a list of
        // read-to-use SampleTypeTraits, see file audio_sample_types.h.
        // If |num_frames_to_write| is less than frames(), the remaining frames are
        // zeroed out. If |num_frames_to_write| is more than frames(), this results in
        // underined behavior.
        template<class SourceSampleTypeTraits>
        void FromInterleaved(
                const typename SourceSampleTypeTraits::ValueType* source_buffer,
                int num_frames_to_write);

        // Similar to FromInterleaved...(), but overwrites the frames starting at a
        // given offset |write_offset_in_frames| and doest not zero out frames that are
        // not overwritten.
        template<class SourceSampleTypeTraits>
        void FromInterleavedPartial(
                const typename SourceSampleTypeTraits::ValueType* source_buffer,
                int write_offset_in_frames,
                int num_frames_to_write);

        // Reads the sample values stored in this AudioBus instance and places them
        // into the given |dest_buffer| in interleaved format using the sample forat
        // specified by TargetSampleTypeTraits. For a list of ready-to-use
        // SampleTypeTraits, see file audo_sample_types.h. if |num_frames_to_read) is
        // larger than frames(), this results in undefined behavior.
        template<class TargetSampleTypeTraits>
        void ToInterleaved(
                int num_frames_to_read,
                typename TargetSampleTypeTraits::ValueType* dest_buffer) const;

        // Similar to ToInterleaved(), but reads the frames starting at a given
        // offset |read_offset_in_frames|.
        template<class TargetSampleTypeTraits>
        void ToInterleavedPartial(
                int read_offset_in_frames,
                int num_frames_to_read,
                typename TargetSampleTypeTraits::ValueType* dest_buffer) const;

        // Helper method for copying channel data from one AudioBus to another.
        // Both AudioBus object must have the same frames() and channels().
        void CopyTo(AudioBus* dest) const;

        // Similar to above, but clips values to [-1. +1] during the copy process.
        void CopyAndClipto(AudioBus* dest) const;

        // Helper method to copy frames from one AudioBus to another. Both AudioBus
        // objects must have the same number of channels(). |source_start_frame| is
        // the starting offset. |dest_start_frame| is the starting offset in |dest|.
        // |frame_count| is the number of frames to copy.
        void CopyPartialFrameTo(int source_start_frame,
                                int frame_count,
                                int dest_start_frame,
                                AudioBus* dest) const;

        // Returns a raw pointer to the request channel. Pointer is guaranteed to
        // have a 16-byte alignment. Warning: Do not rely on having sane (i.e. not
        // inf, nan, or between [-1.0, 1.0]) values in the channel data.
        float* channel(int channel) { return channel_data_[channel]; }

        const float* channel(int channel) const { return channel_data_[channel]; }

        // Returns the number of channels.
        int channels() const { return static_cast<int>(channel_data_.size()); }

        // Returns the number of frames.
        int frames() const { return frames_; }

        // Helper method for zeroing out all channels of audio data.
        void Zero();

        void ZeroFrames(int frames);

        void ZeroFramesPartial(int start_frame, int frames);

        // Checks if all frames are zero.
        bool AreFramesZero() const;

        // Scale internal channel values by |volume| >= 0. If an invalid value
        // privided, no adjustment is done.
        void Scale(float volume);

        // Swaps channels identified by |a| and |b|. The caller neeeds to make sure
        // the channels are valid.
        void SwapChannels(int a, int b);

        AudioBus(const AudioBus&) = delete;

        AudioBus& operator=(const AudioBus&) = delete;

        virtual ~AudioBus();

    protected:
        AudioBus(int channels, int frames);

        AudioBus(int channels, int frames, float* data);

        AudioBus(int frames, const std::vector<float*>& channel_data);

        explicit AudioBus(int channels);

    private:
        void BuildChannelData(int channels, int aligned_frame, float* data);

        static void CheckOverflow(int start_frame, int frames, int total_frames);

        template<class SourceSampleTypeTraits>
        static void CopyConvertFromInterleavedSourceToAudioBus(
                const typename SourceSampleTypeTraits::ValueType* source_buffer,
                int write_offset_in_frames,
                int num_frames_to_write,
                AudioBus* dest);

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
