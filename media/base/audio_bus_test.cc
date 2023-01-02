//
// Created by wangrl2016 on 2022/12/31.
//

#include <gtest/gtest.h>
#include "media/base/audio_bus.h"
#include "media/base/audio_parameters.h"
#include "media/base/channel_layout.h"

namespace media {
    static const int kChannels = 6;
    static constexpr ChannelLayout kChannelLayout = CHANNEL_LAYOUT_5_1;
    // Use a buffer size which is intenionally not a multiple of kChannelAlignment.
    static const int kFrameCount = AudioBus::kChannelAlignment * 32 - 1;
    static const int kSampleRate = 48000;

    class AudioBusTest : public testing::Test {
    public:
        AudioBusTest() = default;

        AudioBusTest(const AudioBusTest&) = delete;

        AudioBusTest& operator=(const AudioBusTest&) = delete;

        ~AudioBusTest() override {
            for (auto& data: data_) {
                base::AlignedFree(data);
            }
        }

        void VerifyChannelAndFrameCount(AudioBus* bus) {
            EXPECT_EQ(kChannels, bus->channels());
            EXPECT_EQ(kFrameCount, bus->frames());
        }

        void VerifyArrayIsFilledWithValue(const float data[], int size, float value) {
            for (int i = 0; i < size; i++) {
                ASSERT_FLOAT_EQ(value, data[i]) << 'i = ' << i;
            }
        }

        // Read and write to the full extent of the allocated channel data. Also test
        // the Zero() method and verify it does as advertised. Also test data if data
        // is 16-byte aligned as advertised (see kChannelAlignment in audio_bus.h).
        void VerifyReadWriteAndAlignment(AudioBus* bus) {
            for (int i = 0; i < bus->channels(); i++) {
                // Verify that the address returned by channel(i) is a multiple of
                // AudioBus::kChannelAlignment.
                ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(
                                      bus->channel(i)) & (AudioBus::kChannelAlignment - 1));

                // Write into the channel buffer.
                std::fill(bus->channel(i), bus->channel(i) + bus->frames(), i);
            }

            for (int i = 0; i < bus->channels(); i++) {
                VerifyArrayIsFilledWithValue(bus->channel(i), bus->frames(), i);
            }

            bus->Zero();
            for (int i = 0; i < bus->channels(); i++) {
                VerifyArrayIsFilledWithValue(bus->channel(i), bus->frames(), 0);
            }
        }

    protected:
        std::vector<float*> data_;
    };

    // Verify basic Create(...) method workds as advertised.
    TEST_F(AudioBusTest, Create) {
        std::unique_ptr<AudioBus> bus = AudioBus::Create(kChannels, kFrameCount);
        VerifyChannelAndFrameCount(bus.get());
        VerifyReadWriteAndAlignment(bus.get());
    }

    // Verify Create(...) using AudioParameters works as advertised.
    TEST_F(AudioBusTest, CreateUsingAudioParameters) {
        std::unique_ptr<AudioBus> bus = AudioBus::Create(
                AudioParameters(AudioParameters::AUDIO_PCM_LINEAR,
                                ChannelLayoutConfig::FromLayout<kChannelLayout>(),
                                kSampleRate,
                                kFrameCount));
        VerifyChannelAndFrameCount(bus.get());
        VerifyReadWriteAndAlignment(bus.get());
    }

    TEST_F(AudioBusTest, CreateWrapper) {
        // Test里面的数据结构
        data_.reserve(kChannels);
        for (int i = 0; i < kChannels; i++) {
            data_.push_back(static_cast<float*>(base::AlignedAlloc(
                    sizeof(*data_[i]) * kFrameCount, AudioBus::kChannelAlignment)));
        }

        std::unique_ptr<AudioBus> bus = AudioBus::CreateWrapper(kChannels);
        bus->set_frames(kFrameCount);
        for (int i = 0; i < bus->channels(); i++) {
            if (data_[i] == nullptr) {
                LOG(INFO) << "data is nullptr";
            }
            bus->SetChannelData(i, data_[i]);
        }

        bool deleted = false;
        bus->SetWrappedDataDeleter([&deleted]() -> void {
            deleted = true;
        });

        VerifyChannelAndFrameCount(bus.get());
        VerifyReadWriteAndAlignment(bus.get());

        EXPECT_FALSE(deleted);
        bus.reset();
        EXPECT_TRUE(deleted);
    }

}


