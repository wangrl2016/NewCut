//
// Created by wangrl2016 on 2022/12/30.
//

#include <gtest/gtest.h>
#include "media/filters/audio_file_reader.h"
#include "media/filters/memory_url_protocol.h"
#include "media/base/decoder_buffer.h"
#include "media/base/test_data_util.h"

namespace media {
    class AudioFileReaderTest : public testing::Test {
    public:
        AudioFileReaderTest() : packet_verfication_disabled_(false) {}

        AudioFileReaderTest(const AudioFileReaderTest&) = delete;

        AudioFileReaderTest& operator=(const AudioFileReaderTest&) = delete;

        ~AudioFileReaderTest() override = default;

        void Initialize(const char* filename) {
            data_ = ReadTestDataFile(filename);
        }

    protected:
        std::shared_ptr<DecoderBuffer> data_;
        std::unique_ptr<MemoryURLProtocol> protocol_;
        std::unique_ptr<AudioFileReader> reader_;
        bool packet_verfication_disabled_;
    };
}
