//
// Created by wangrl2016 on 2022/12/17.
//

#include "media/base/audio_bus.h"
#include "media/base/limits.h"
#include "media/base/audio_parameters.h"

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

    void AudioBus::CheckOverflow(int start_frame, int frames, int total_frames) {
        CHECK_GE(start_frame, 0);
        CHECK_GE(frames, 0);
        CHECK_GT(total_frames, 0);
        int sum = start_frame + frames;
        CHECK_LE(sum, total_frames);
        CHECK_GE(sum, 0);
    }

    std::unique_ptr<AudioBus> AudioBus::Create(int channels, int frames) {
        return std::unique_ptr<AudioBus>(new AudioBus(channels, frames));
    }

    std::unique_ptr<AudioBus> AudioBus::Create(const media::AudioParameters& params) {
        return std::unique_ptr<AudioBus>(
                new AudioBus(params.channels(), params.frames_per_buffer()));
    }

    std::unique_ptr<AudioBus> AudioBus::CreateWrapper(int channels) {
        return std::unique_ptr<AudioBus>(new AudioBus(channels));
    }

    std::unique_ptr<AudioBus> AudioBus::WrapVector(
            int frames,
            const std::vector<float*>& channel_data) {
        return std::unique_ptr<AudioBus>(new AudioBus(frames, channel_data));
    }

    std::unique_ptr<AudioBus> AudioBus::WrapMemory(int channels,
                                                   int frames,
                                                   void* data) {
        // |data| must be aligned by AudioBus::kChannelAlignment.
        CHECK(IsAligned(data));
        return std::unique_ptr<AudioBus>(
                new AudioBus(channels, frames, static_cast<float*>(data)));
    }

    std::unique_ptr<AudioBus> AudioBus::WrapMemory(const media::AudioParameters& params,
                                                   void* data) {
        // |data| must be aligned by AudioBus::kChannelAlignment.
        CHECK(IsAligned(data));
        return std::unique_ptr<AudioBus>(new AudioBus(params.channels(),
                                                      params.frames_per_buffer(),
                                                      static_cast<float*>(data)));
    }

    std::unique_ptr<const AudioBus> AudioBus::WrapReadOnlyMemory(
            int channels,
            int frames,
            const void* data) {
        // Note: const_cast is generally dangerous but is used in this case since
        // AudioBus accomodates both read-only and read-write use cases. A const
        // AudioBus object is returned to ensure no one accidentally writes to the
        // read-only data.
        return WrapMemory(channels, frames, const_cast<void*>(data));
    }

    std::unique_ptr<const AudioBus> AudioBus::WrapReadOnlyMemory(
            const media::AudioParameters& params,
            const void* data) {
        // Note: const_cast is generally dangerous but is used in this case since
        // AudioBus accomodates both read-only and read-write use cases. A const
        // AudioBus object is returned to ensure no one accidentally writes to the
        // read-only data.
        return WrapMemory(params, const_cast<void*>(data));
    }

    int AudioBus::CalculateMemorySize(int channels, int frames) {
        return CalculateMemorySizeInternal(channels, frames, nullptr);
    }

    int AudioBus::CalculateMemorySize(const media::AudioParameters& params) {
        return CalculateMemorySizeInternal(
                params.channels(), params.frames_per_buffer(), nullptr);
    }

    void AudioBus::SetChannelData(int channel, float* data) {
        CHECK(is_wrapper_);
        CHECK(data);
        CHECK_GE(channel, 0);
        CHECK_LT(static_cast<size_t>(channel), channel_data_.size());
        DCHECK(IsAligned(data));
        channel_data_[channel] = data;
    }

    void AudioBus::set_frames(int frames) {
        CHECK(is_wrapper_);
        ValidateConfig(static_cast<int>(channel_data_.size()), frames);
        frames_ = frames;
    }

    void AudioBus::SetWrappedDataDeleter(void (* deleter)()) {
        CHECK(is_wrapper_);
        DCHECK(!wrapped_data_deleter_cb_);
        wrapped_data_deleter_cb_ = deleter;
    }

    size_t AudioBus::GetBitstreamDataSize() const {
        DCHECK(is_bitstream_format_);
        return bitstream_data_size_;
    }

    void AudioBus::SetBitstreamDataSize(size_t data_size) {
        DCHECK(is_bitstream_format_);
        bitstream_data_size_ = data_size;
    }

    int AudioBus::GetBitstreamFrames() const {
        DCHECK(is_bitstream_format_);
        return bitstream_frames_;
    }

    void AudioBus::SetBitstreamFrames(int frames) {
        DCHECK(is_bitstream_format_);
        bitstream_frames_ = frames;
    }


}
