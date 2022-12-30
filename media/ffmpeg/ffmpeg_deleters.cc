//
// Created by wangrl2016 on 2022/12/30.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "media/ffmpeg/ffmpeg_deleters.h"

namespace media {
    void ScopedPtrAVFree::operator()(void* x) const {
        av_free(x);
    }

    void ScopedPtrAVFreePacket::operator()(void* x) const {
        auto* packet = static_cast<AVPacket*>(x);
        av_packet_free(&packet);
    }

    void ScopedPtrAVFreeContext::operator()(void* x) const {
        auto* codec_context = static_cast<AVCodecContext*>(x);
        avcodec_free_context(&codec_context);
    }

    void ScopedPtrAVFreeFrame::operator()(void* x) const {
        auto* frame = static_cast<AVFrame*>(x);
        av_frame_free(&frame);
    }
}
