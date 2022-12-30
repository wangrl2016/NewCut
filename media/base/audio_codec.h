//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_AUDIO_CODEC_H
#define NEWCUT_AUDIO_CODEC_H

namespace media {
    enum class AudioCodec {
        kUnknown    = 0,
        kAAC        = 1,
        kMP3        = 2,
        kPCM        = 3,
        kVorbis     = 4,
        kFLAC       = 5,
        kAMR_NB     = 6,
    };
}

#endif //NEWCUT_AUDIO_CODEC_H
