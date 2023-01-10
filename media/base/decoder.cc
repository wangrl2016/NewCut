//
// Created by wangrl2016 on 2023/1/10.
//

#include "media/base/decoder.h"

namespace media {
    Decoder::Decoder() = default;

    Decoder::~Decoder() = default;

    bool Decoder::IsPlatformDecoder() const {
        return false;
    }

    bool Decoder::SupportsDecryption() const {
        return false;
    }

    std::string GetDecoderName(AudioDecoderType type) {
        switch (type) {
            case AudioDecoderType::kUnknown:
                return "Unknown Audio Decoder";
            case AudioDecoderType::kFFmpeg:
                return "FFmpegAudioDecoder";
            default:
                return "Error Audio Decoder Type";
        }
    }

    std::string GetDecoderName(VideoDecoderType type) {
        switch (type) {
            case VideoDecoderType::kUnknown:
                return "Unknown Video Decoder";
            case VideoDecoderType::kFFmpeg:
                return "FFmpegVideoDecoder";
            default:
                return "Error Video Decoder Type";
        }
    }
}
