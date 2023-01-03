//
// Created by wangrl2016 on 2022/12/30.
//

#include "media/base/decoder_buffer.h"

namespace media {
    DecoderBuffer::DecoderBuffer(size_t size)
            : size_(size), side_data_size_(0), is_key_frame_(false) {
        Initialize();
    }

    void DecoderBuffer::Initialize() {
        data_.reset(new uint8_t[size_]);
        if (side_data_size_ > 0) {
            side_data_.reset(new uint8_t[side_data_size_]);
        }
    }
}