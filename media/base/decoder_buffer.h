//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_DECODER_BUFFER_H
#define NEWCUT_DECODER_BUFFER_H

#include <memory>

namespace media {
    // A specialized buffer for interfacing with audio/video decoders.
    //
    // Also includes decoder specifiec functionality for decryption.
    //
    // NOTE: It is illegal to call any method when end_of_stream() is true.
    class DecoderBuffer {
    public:

        struct TimeInfo {
            TimeInfo();

            ~TimeInfo();

            TimeInfo(const TimeInfo&);

            TimeInfo& operator=(const TimeInfo&);

            // Presentation time of the frame.
            int64_t timestamp;

            // Presentation duraiton of the frame.
            int64_t duration;
        };

    protected:

        // Encoded data, if it is stored on the heap.
        std::unique_ptr<uint8_t[]> data_;

    private:
        TimeInfo time_info_;

        // Size of the encoded data.
        size_t  size_;

        // Side data. used for alpha channel in VPx, and for text cues.
        size_t side_data_size = 0;
        std::unique_ptr<uint8_t> side_data_;

        // Whehter the frame was marked as a keyframe in the container.
        bool is_key_frame_ = false;

        // Constructor helper method for memory allocation.
        void Initialize();
    };
}

#endif //NEWCUT_DECODER_BUFFER_H
