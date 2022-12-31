//
// Created by wangrl2016 on 2022/12/31.
//

#ifndef NEWCUT_FFMPEG_COMMON_H
#define NEWCUT_FFMPEG_COMMON_H

#include <memory>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "base/time/time.h"
#include "media/ffmpeg/ffmpeg_deleters.h"
#include "media/base/audio_codec.h"

namespace media {
    static const AVRational kMicrosBase = {1, base::Time::kMicrosecondPerSecond};

    // Converts an int64_t timestamp in |time_base| units to an int64_t value.
    // For example if |timestamp| equals 11025 and |time_base| equals {1, 44100}
    // then the return value will be an int64_t for 0.25 seconds since that
    // is how much time 11025/44100ths of a second represents.
    int64_t ConvertFromTimeBase(const AVRational& time_base,
                                int64_t timestamp);

    // Converts a timestamp into an int64_t timestamp in |time_base| units.
    // For xample if |timestamp| is 0.5 second and |time_base| is {1, 44100}, then
    // the return value will be 22050 since that is how many 1/44100ths of a second
    // represent 0.5 seconds.
    int64_t ConvertToTimeBase(const AVRational& time_base,
                              int64_t timestamp);

    // Converts an FFmpeg audio codec ID into its corresponding supported codec id.
    AudioCodec CodecIDToAudioCodec(AVCodecID codec_id);

    // Allocates, populates and returns a wrapped AVCodecContext from the
    // AVCodecParameters in |stream|. On failure, returns a wrapped nullptr.
    // Wrapping helps ensure eventual destruction of the AVCodecContext.
    std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext>
    AVStreamToAVCodecContext(const AVStream* stream);
}

#endif //NEWCUT_FFMPEG_COMMON_H
