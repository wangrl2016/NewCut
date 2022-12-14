//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_DECODER_BUFFER_H
#define NEWCUT_DECODER_BUFFER_H

#include <memory>
#include <glog/logging.h>

namespace media {
    // A specialized buffer for interfacing with audio/video decoders.
    //
    // Also includes decoder specific functionality for decryption.
    //
    // NOTE: It is illegal to call any method when end_of_stream() is true.
    class DecoderBuffer {
    public:

        struct TimeInfo {
            TimeInfo() = default;

            ~TimeInfo() = default;

            TimeInfo(const TimeInfo&) = default;

            TimeInfo& operator=(const TimeInfo&) = default;

            // Presentation time of the frame.
            int64_t timestamp;

            // Presentation duration of the frame.
            int64_t duration;
        };

        // Allocates buffer with |size| >= 0. |is_key_frame_| will default to false.
        explicit DecoderBuffer(size_t size);

        DecoderBuffer(const DecoderBuffer&) = delete;

        DecoderBuffer& operator=(const DecoderBuffer&) = delete;

        const uint8_t* data() const {
            DCHECK(!end_of_stream());
            return data_.get();
        }

        uint8_t* writable_data() const {
            DCHECK(!end_of_stream());
            return data_.get();
        }

        size_t data_size() const {
            DCHECK(!end_of_stream());
            return size_;
        }

        // If there's no data in this buffer, it represents end of stream.
        bool end_of_stream() const {
            return !data_;
        }

    protected:
        // Encoded data, if it is stored on the heap.
        std::unique_ptr<uint8_t[]> data_;

    private:
        TimeInfo time_info_;

        // Size of the encoded data.
        size_t size_;

        // Side data. used for alpha channel in VPx, and for text cues.
        size_t side_data_size_ = 0;
        std::unique_ptr<uint8_t> side_data_;

        // Whether the frame was marked as a keyframe in the container.
        bool is_key_frame_ = false;

        // Constructor helper method for memory allocation.
        void Initialize();
    };
}

#endif //NEWCUT_DECODER_BUFFER_H
