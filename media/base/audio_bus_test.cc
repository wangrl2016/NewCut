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

        // Verify values for each channel in |result| are within |epsilon| of
        // |expected|. If |epsilon| exactly equals 0. uses FLOAT_EQ macro.
        void VerifyAreEqualWithEpsilon(const AudioBus* result,
                                       const AudioBus* expected,
                                       float epsilon) {
            ASSERT_EQ(expected->channels(), result->channels());
            ASSERT_EQ(expected->frames(), result->frames());
            ASSERT_EQ(expected->is_bitstram_format(), result->is_bitstram_format());

            if (expected->is_bitstram_format()) {
                ASSERT_EQ(expected->GetBitstreamDataSize(),
                          result->GetBitstreamDataSize());
                ASSERT_EQ(expected->GetBitstreamFrames(),
                          result->GetBitstreamFrames());
                ASSERT_EQ(0, memcmp(expected->channel(0), result->channel(0),
                                    result->GetBitstreamDataSize()));
                return;
            }

            for (int ch = 0; ch < result->channels(); ch++) {
                for (int i = 0; i < result->frames(); i++) {
                    if (epsilon == 0) {
                        ASSERT_FLOAT_EQ(expected->channel(ch)[i], result->channel(ch)[i]);
                    } else {
                        ASSERT_NEAR(expected->channel(ch)[i], result->channel(ch)[i],
                                    epsilon);
                    }
                }
            }
        }

        // Verify values for each channel in |result| against |expected|.
        void VerifyAreEqual(const AudioBus* result, const AudioBus* expected) {
            VerifyAreEqualWithEpsilon(result, expected, 0);
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

        // Verify copying to and from |bus1| and |bus2|.
        void CopyTest(AudioBus* bus1, AudioBus* bus2) {
            // Fill |bus1| with dummy data.
            for (int i = 0; i < bus1->channels(); i++) {
                std::fill(bus1->channel(i), bus1->channel(i) + bus1->frames(), i);
            }

            // Verify copy from |bus1| to |bus2|.
            bus2->Zero();
            bus1->CopyTo(bus2);
            VerifyAreEqual(bus1, bus2);

            // Verify copy from |bus2| to |bus1|.
            bus1->Zero();
            bus2->CopyTo(bus1);
            VerifyAreEqual(bus2, bus1);
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

    // Verify an AudioBus created via CreateWrapper(...) works as advertied.
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

    // Verify an AudioBus create via wrapping a vector works as advertised.
    TEST_F(AudioBusTest, WrapVector) {
        data_.reserve(kChannels);
        for (int i = 0; i < kChannels; i++) {
            data_.push_back(static_cast<float*>(base::AlignedAlloc(
                    sizeof(*data_[i]) * kFrameCount, AudioBus::kChannelAlignment)));
        }

        std::unique_ptr<AudioBus> bus = AudioBus::WrapVector(kFrameCount, data_);
        VerifyChannelAndFrameCount(bus.get());
        VerifyReadWriteAndAlignment(bus.get());
    }

    TEST_F(AudioBusTest, WrapMemory) {
        AudioParameters params(AudioParameters::AUDIO_PCM_LINEAR,
                               ChannelLayoutConfig::FromLayout<kChannelLayout>(),
                               kSampleRate,
                               kFrameCount);
        int data_size = AudioBus::CalculateMemorySize(params);
        std::unique_ptr<float, base::AlignedFreeDeleter> data(
                static_cast<float*>(
                        base::AlignedAlloc(data_size,
                                           AudioBus::kChannelAlignment)));

        // Fill the memory with a test value we can check for after wrapping.
        static const float kTestValue = 3;
        std::fill(
                data.get(), data.get() + data_size / sizeof(*data.get()), kTestValue);

        std::unique_ptr<AudioBus> bus = AudioBus::WrapMemory(params, data.get());
        // Verify the test value we filled prior to wrapping.
        for (int i = 0; i < bus->channels(); i++) {
            VerifyArrayIsFilledWithValue(bus->channel(i), bus->frames(), kTestValue);
        }
        VerifyChannelAndFrameCount(bus.get());
        VerifyReadWriteAndAlignment(bus.get());

        // Verify the channel vectors lie within the provided memory block.
        EXPECT_GE(bus->channel(0), data.get());
        EXPECT_LT(bus->channel(bus->channels() - 1) + bus->frames(),
                  data.get() + data_size / sizeof(*data.get()));
    }
}
