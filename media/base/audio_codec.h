//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_AUDIO_CODEC_H
#define NEWCUT_AUDIO_CODEC_H

#include <string>
#include <ostream>

namespace media {
    enum class AudioCodec {
        kUnknown    = 0,
        kAAC        = 1,
        kMP3        = 2,
        kPCM        = 3,
        kVorbis     = 4,
        kFLAC       = 5,
        kAMR_NB     = 6,
        kAMR_WB     = 7,
        kPCM_MULAW  = 8,
        kGSM_MS     = 9,
        kPCM_S16BE  = 10,
        kPCM_S24BE  = 11,
        kOpus       = 12,
        kEAC3       = 13,
        kPCM_ALAW   = 14,
        kALAC       = 15,
        kAC3        = 16,
        kMpegHAudio = 17,
        kDTS        = 18,
        kDTSXP2     = 19,
        // DO NOT AND RANDOM AUDIO CODECS!
        //
        // The only acceptable time to add a new codec is if there is production code
        // that uses said codec in the same CL.

        // Must always be equal to the largest enty ever logged.
        kMaxValue = kDTSXP2,
    };

    std::string GetCodecName(AudioCodec codec);

    std::ostream& operator<<(std::ostream& os,
            const AudioCodec& codec);

    AudioCodec StringToAudioCodec(const std::string& codec_d);
}

#endif //NEWCUT_AUDIO_CODEC_H
