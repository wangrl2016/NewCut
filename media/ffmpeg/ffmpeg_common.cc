//
// Created by wangrl2016 on 2022/12/31.
//

#include <glog/logging.h>
#include "media/ffmpeg/ffmpeg_common.h"

namespace media {

    int64_t ConvertFromTimeBase(const AVRational& time_base,
                                int64_t timestamp) {
        return av_rescale_q(timestamp, time_base, kMicrosBase);
    }

    int64_t ConvertToTimeBase(const AVRational& time_base,
                              int64_t timestamp) {
        return av_rescale_q(timestamp, kMicrosBase, time_base);
    }

    AudioCodec CodecIDToAudioCodec(AVCodecID codec_id) {
        switch (codec_id) {
            case AV_CODEC_ID_AAC:
                return AudioCodec::kAAC;
            case AV_CODEC_ID_AC3:
                return AudioCodec::kAC3;
            case AV_CODEC_ID_MP3:
                return AudioCodec::kMP3;
            case AV_CODEC_ID_VORBIS:
                return AudioCodec::kVorbis;
            case AV_CODEC_ID_PCM_U8:
            case AV_CODEC_ID_PCM_S16LE:
            case AV_CODEC_ID_PCM_S24LE:
            case AV_CODEC_ID_PCM_S32LE:
            case AV_CODEC_ID_PCM_F32LE:
                return AudioCodec::kPCM;
            case AV_CODEC_ID_PCM_S16BE:
                return AudioCodec::kPCM_S16BE;
            case AV_CODEC_ID_PCM_S24BE:
                return AudioCodec::kPCM_S24BE;
            case AV_CODEC_ID_FLAC:
                return AudioCodec::kFLAC;
            case AV_CODEC_ID_PCM_MULAW:
                return AudioCodec::kPCM_MULAW;
            case AV_CODEC_ID_OPUS:
                return AudioCodec::kOpus;
            case AV_CODEC_ID_ALAC:
                return AudioCodec::kALAC;
            case AV_CODEC_ID_MPEGH_3D_AUDIO:
                return AudioCodec::kMpegHAudio;
            default:
                LOG(WARNING) << "Unknown audio CodecID: " << codec_id;
        }
        return AudioCodec::kUnknown;
    }

    std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext>
    AVStreamToAVCodecContext(const AVStream* stream) {
        std::unique_ptr<AVCodecContext, ScopedPtrAVFreeContext> codec_context(
                avcodec_alloc_context3(nullptr));
        if (avcodec_parameters_to_context(codec_context.get(), stream->codecpar) < 0) {
            return nullptr;
        }
        return codec_context;
    }
}