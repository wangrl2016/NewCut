//
// Created by wangrl2016 on 2022/12/31.
//

#ifndef NEWCUT_FFMPEG_COMMON_H
#define NEWCUT_FFMPEG_COMMON_H

#include <memory>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
};
#include "media/ffmpeg/ffmpeg_deleters.h"
#include "media/base/audio_codec.h"

namespace media {
    // Converts an FFmpeg audio codec ID into its corresponding supported codec id.
    AudioCodec CodecIDToAudioCodec(AVCodecID codec_id);

    // Allocates, populates and returns a wrapped AVCodecContext from the
    // AVCodecParameters in |stream|. On failure, returns a wrapped nullptr.
    // Wrapping helps ensure eventual destruction of the AVCodecContext.
    std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext>
            AVStreamToAVCodecContext(const AVStream* stream);
}

#endif //NEWCUT_FFMPEG_COMMON_H
