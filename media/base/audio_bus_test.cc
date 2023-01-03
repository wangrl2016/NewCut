//
// Created by wangrl2016 on 2022/12/31.
//

#include <gtest/gtest.h>
#include "media/base/audio_bus.h"
#include "media/base/audio_parameters.h"
#include "media/base/audio_sample_types.h"
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

    // Simulate a shared memory transfer and verify results.
    TEST_F(AudioBusTest, CopyTo) {
        // Create one bus with AudioParameters and the other through direct values to
        // test for parity between the Create() functions.
        AudioParameters params(AudioParameters::AUDIO_PCM_LINEAR,
                               ChannelLayoutConfig::FromLayout<kChannelLayout>(),
                               kSampleRate,
                               kFrameCount);

        std::unique_ptr<AudioBus> bus1 = AudioBus::Create(kChannels, kFrameCount);
        std::unique_ptr<AudioBus> bus2 = AudioBus::Create(params);

        {
            SCOPED_TRACE("Created");
            CopyTest(bus1.get(), bus2.get());
        }
        {
            SCOPED_TRACE("Wrapped Vector");
            // Try a copy to an AudioBus wrapping a vector.
            data_.reserve(kChannels);
            for (int i = 0; i < kChannels; i++) {
                data_.push_back(static_cast<float*>(base::AlignedAlloc(
                        sizeof(*data_[i]) * kFrameCount, AudioBus::kChannelAlignment)));
            }

            bus2 = AudioBus::WrapVector(kFrameCount, data_);
            CopyTest(bus1.get(), bus2.get());
        }
        {
            SCOPED_TRACE("Wrapped Memory");
            // Try a copy to an AudioBus wrapping a memory block.
            std::unique_ptr<float, base::AlignedFreeDeleter> data(
                    static_cast<float*>(
                            base::AlignedAlloc(
                                    AudioBus::CalculateMemorySize(params),
                                    AudioBus::kChannelAlignment)));

            bus2 = AudioBus::WrapMemory(params, data.get());
            CopyTest(bus1.get(), bus2.get());
        }
    }

    // Verify Zero() and ZeroFrames(...) utility methods work as advertised.
    TEST_F(AudioBusTest, Zero) {
        std::unique_ptr<AudioBus> bus = AudioBus::Create(kChannels, kFrameCount);

        // Fill the bus with dummy data.
        for (int i = 0; i < bus->channels(); i++) {
            std::fill(bus->channel(i), bus->channel(i) + bus->frames(), i + 1);
        }
        EXPECT_FALSE(bus->AreFramesZero());

        // Zero first half the frames of each channel.
        bus->ZeroFrames(kFrameCount / 2);
        for (int i = 0; i < bus->channels(); i++) {
            SCOPED_TRACE("First Half Zero");
            VerifyArrayIsFilledWithValue(bus->channel(i), kFrameCount / 2, 0);
            VerifyArrayIsFilledWithValue(bus->channel(i) + kFrameCount / 2,
                                         kFrameCount - kFrameCount / 2, i + 1);
        }
        EXPECT_FALSE(bus->AreFramesZero());

        // Fill the bus with dummy data.
        for (int i = 0; i < bus->channels(); i++) {
            std::fill(bus->channel(i), bus->channel(i) + bus->frames(), i + 1);
        }

        // Zero all the frames of each channel.
        bus->Zero();
        for (int i = 0; i < bus->channels(); i++) {
            SCOPED_TRACE("All Zero");
            VerifyArrayIsFilledWithValue(bus->channel(i), bus->frames(), 0);
        }
        EXPECT_TRUE(bus->AreFramesZero());
    }

    // Each test vector represents two channels of data in the following arbitrary.
    // layout: <min, zero, max, min, max / 2, min / 2, zero, max, zero, zero>
    static const int kTestVectorSize = 10;
    static const uint8_t kTestVectorUint8[kTestVectorSize] = {
            0, - INT8_MIN, UINT8_MAX,
            0, INT8_MAX / 2 + 128, INT8_MIN / 2 + 128,
            - INT8_MIN, UINT8_MAX, - INT8_MIN,
            - INT8_MIN};
    static const int16_t kTestVectorInt16[kTestVectorSize] = {
            INT16_MIN, 0, INT16_MAX, INT16_MIN, INT16_MAX / 2,
            INT16_MIN / 2, 0, INT16_MAX, 0, 0
    };
    static const int32_t kTestVectorInt32[kTestVectorSize] = {
            INT32_MIN, 0, INT32_MAX, INT32_MIN, INT32_MAX / 2,
            INT32_MIN / 2, 0, INT32_MAX, 0, 0
    };
    static const float kTestVectorFloat32[kTestVectorSize] = {
            -1.0f, 0.0f, 1.0f, -1.0f, 0.5f,
            -0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    // This is based on kTestVectorFloat32, but has some values outside of
    // sanity.
    static const float kTestVectorFloat32Invalid[kTestVectorSize] = {
            -5.0f, 0.0f, 5.0f, -1.0f, 0.5f, -0.5f, 0.0f,
            std::numeric_limits<float>::infinity(),
            std::numeric_limits<float>::signaling_NaN(),
            std::numeric_limits<float>::quiet_NaN()
    };
    static const float kTestVectorFloat32Sanitized[kTestVectorSize] = {
            -1.0f, 0.0f, 1.0f, -1.0f, 0.5f,
            -0.5f, 0.0f, 1.0f, -1.0f, -1.0f
    };

    // Expected results.
    static const int kTestVectorFrameCount = kTestVectorSize / 2;
    // 双声道存储
    static const float kTestVectorResult[][kTestVectorFrameCount] = {
            {-1.0f, 1.0f, 0.5f, 0.0f, 0.0f},
            {0.0f, -1.0f, -0.5f, 1.0f, 0.0f}
    };

    static const int kTestVectorChannelCount = std::size(kTestVectorResult);

    // Verify FromInterleaved() deinterleaves audio in supported format correctly.
    TEST_F(AudioBusTest, FromInterleaved) {
        std::unique_ptr<AudioBus> bus =
                AudioBus::Create(kTestVectorChannelCount, kTestVectorFrameCount);
        std::unique_ptr<AudioBus> expected =
                AudioBus::Create(kTestVectorChannelCount, kTestVectorFrameCount);
        for (int ch = 0; ch < kTestVectorChannelCount; ++ch) {
            memcpy(expected->channel(ch), kTestVectorResult[ch],
                   kTestVectorFrameCount * sizeof(*expected->channel(ch)));
        }
        {
            SCOPED_TRACE("UnsignedInt8SampleTypeTraits");
            bus->Zero();
            bus->FromInterleaved<UnsignedInt8SampleTypeTraits>(
                    kTestVectorUint8,
                    kTestVectorFrameCount);
            // Biased uint8_t calculations have poor precision, so the epsilon here is
            // slightly more permissive than int16_t and int32_t calculations.
            VerifyAreEqualWithEpsilon(bus.get(), expected.get(),
                                      1.0f / (std::numeric_limits<uint8_t>::max() - 1));
        }
        {
            SCOPED_TRACE("SignedInt16SampleTypeTraits");
            bus->Zero();
            bus->FromInterleaved<SignedInt16SampleTypeTraits>(
                    kTestVectorInt16,
                    kTestVectorFrameCount);
            VerifyAreEqualWithEpsilon(
                    bus.get(), expected.get(),
                    1.0f / (static_cast<float>(std::numeric_limits<uint16_t>::max()) + 1.0f));
        }
        {
            SCOPED_TRACE("SignedInt32SampleTypeTraits");
            bus->Zero();
            bus->FromInterleaved<SignedInt32SampleTypeTraits>(kTestVectorInt32,
                                                              kTestVectorFrameCount);
            VerifyAreEqualWithEpsilon(
                    bus.get(), expected.get(),
                    1.0f / static_cast<float>(std::numeric_limits<uint32_t>::max()));
        }
        {
            SCOPED_TRACE("Float32SampleTypeTraits");
            bus->Zero();
            bus->FromInterleaved<Float32SampleTypeTraits>(kTestVectorFloat32,
                                                          kTestVectorFrameCount);
            VerifyAreEqual(bus.get(), expected.get());
        }
    }

    // Verify FromInterleavedpartial() deinterleaves audio correctly.
    TEST_F(AudioBusTest, FromInterleavedPartial) {
        // Only deinterleave the middle two frames in each channel.
        static const int kPartialStart = 1;
        static const int kPartialFrames = 2;
        ASSERT_LE(kPartialStart + kPartialFrames, kTestVectorFrameCount);

        std::unique_ptr<AudioBus> bus =
                AudioBus::Create(kTestVectorChannelCount, kTestVectorFrameCount);
        std::unique_ptr<AudioBus> expected =
                AudioBus::Create(kTestVectorChannelCount, kTestVectorFrameCount);
        expected->Zero();
        for (int ch = 0; ch < kTestVectorChannelCount; ch++) {
            memcpy(expected->channel(ch) + kPartialStart,
                   kTestVectorResult[ch] + kPartialStart,
                   kPartialFrames * sizeof(*expected->channel(ch)));
        }
        {
            SCOPED_TRACE("SignedInt32SampleTypeTraits");
            bus->Zero();
            bus->FromInterleavedPartial<SignedInt32SampleTypeTraits>(
                    kTestVectorInt32 + kPartialStart * bus->channels(),
                    kPartialStart,
                    kPartialFrames);
            VerifyAreEqual(bus.get(), expected.get());
        }
    }

    // Verify ToInterleaved() interleaves audio in supported formats correctly.
    TEST_F(AudioBusTest, ToInterleaved) {
        std::unique_ptr<AudioBus> bus =
                AudioBus::Create(kTestVectorChannelCount, kTestVectorFrameCount);
        // Fill the bus with our test vector.
        for (int ch = 0; ch < bus->channels(); ch++) {
            memcpy(bus->channel(ch), kTestVectorResult[ch],
                   kTestVectorFrameCount * sizeof(*bus->channel(ch)));
        }
        {
            SCOPED_TRACE("UnsignedInt8SampleTypeTraits");
            uint8_t test_array[std::size(kTestVectorUint8)];
            bus->ToInterleaved<UnsignedInt8SampleTypeTraits>(bus->frames(), test_array);
            ASSERT_EQ(0, memcmp(test_array, kTestVectorUint8, sizeof(kTestVectorUint8)));
        }
        {
            SCOPED_TRACE("SingedInt16SampleTypeTraits");
            int16_t test_array[std::size(kTestVectorInt16)];
            bus->ToInterleaved<SignedInt16SampleTypeTraits>(bus->frames(), test_array);
        }
    }
}
