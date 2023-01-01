//
// Created by wangrl2016 on 2022/12/31.
//

#include <gtest/gtest.h>
#include "media/base/audio_bus.h"
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
            for (auto & data : data_) {
                base::AlignedFree(data);
            }
        }

        void VerifyChannelAndFrameCount(AudioBus* bus) {
            EXPECT_EQ(kChannels, bus->channels());
        }

    protected:
        std::vector<float*> data_;
    };

    // Verify basic Create(...) method workds as advertised.
    TEST_F(AudioBusTest, Create) {

    }
}
