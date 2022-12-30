//
// Created by wangrl2016 on 2022/12/30.
//

#include <gtest/gtest.h>
#include "media/filters/audio_file_reader.h"
#include "media/filters/memory_url_protocol.h"
#include "media/base/decoder_buffer.h"

namespace media {
    class AudioFileReaderTest : public testing::Test {
    public:

    protected:
        DecoderBuffer* data_;
        std::unique_ptr<MemoryURLProtocol> protocol_;
        std::unique_ptr<AudioFileReader> reader_;
        bool packet_verfication_disabled_;
    };
}
