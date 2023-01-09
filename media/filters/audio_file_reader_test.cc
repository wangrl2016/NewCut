//
// Created by wangrl2016 on 2022/12/30.
//

#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include "media/base/audio_bus.h"
#include "media/base/decoder_buffer.h"
#include "media/base/test_data_util.h"
#include "media/filters/audio_file_reader.h"
#include "media/filters/memory_url_protocol.h"

namespace media {
    class AudioFileReaderTest : public testing::Test {
    public:
        AudioFileReaderTest() : packet_verification_disabled_(false) {}

        AudioFileReaderTest(const AudioFileReaderTest&) = delete;

        AudioFileReaderTest& operator=(const AudioFileReaderTest&) = delete;

        ~AudioFileReaderTest() override = default;

        void Initialize(const char* filename) {
            data_ = ReadTestDataFile(filename);
            protocol_ = std::make_unique<MemoryURLProtocol>(
                    data_->data(), data_->data_size(), false);
            reader_ = std::make_unique<AudioFileReader>(protocol_.get());
        }

        // Reads and the entire file provided to Initialize().
        void ReadAndVerify(const char* expected_audio_hash, int expected_frames) {
            std::vector<std::unique_ptr<AudioBus>> decoded_audio_packets;
            int actual_frames = reader_->Read(&decoded_audio_packets);
            std::unique_ptr<AudioBus> decoded_audio_data =
                    AudioBus::Create(reader_->channels(), actual_frames);
            int dest_start_frame = 0;

        }

        void VerifyPackets() {

        }

        void RunTest() {

        }

        void RunTestFailingDemux(const char* fn) {
            Initialize(fn);
            EXPECT_FALSE(reader_->Open());
        }

        void RunTestFailingDecode(const char* fn, int expect_read = 0) {
            Initialize(fn);
            EXPECT_TRUE(reader_->Open());
        }

    protected:
        std::shared_ptr<DecoderBuffer> data_;
        std::unique_ptr<MemoryURLProtocol> protocol_;
        std::unique_ptr<AudioFileReader> reader_;
        bool packet_verification_disabled_;
    };

    TEST_F(AudioFileReaderTest, WithoutOpen) {
        Initialize("utf8.png");
    }

    TEST_F(AudioFileReaderTest, InvalidFile) {
        RunTestFailingDemux("ten_byte_file");
    }
}
