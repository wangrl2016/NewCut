//
// Created by wangrl2016 on 2022/12/21.
//

#ifndef NEWCUT_CONTAINER_NAMES_H
#define NEWCUT_CONTAINER_NAMES_H

#include <cstdint>

namespace media {
    enum MediaContainerName {
        CONTAINER_UNKNOWN,          // Unknown
        CONTAINER_AAC,              // AAC (Advanced Audio Coding)
        CONTAINER_AC3,              // AC-3
        CONTAINER_AIFF,             // AIFF (Audio Interchange File Format)
        CONTAINER_AMR,              // AMR (Adaptive Multi-Rate Audio)
        CONTAINER_APE,              // APE (Monkey's Audio)
        CONTAINER_ASF,              // ASF (Advanced / Active Streaming Format)
        CONTAINER_ASS,              // SSA (SubStation Alpha) subtitle
        CONTAINER_AVI,              // AVI (Audio Video Interleaved)
        CONTAINER_BINK,             // Bink
        CONTAINER_CAF,              // CAF (App Core Audio Format)
        CONTAINER_DTS,              // DTS
        CONTAINER_DTSHD,            // DTS-HD
        CONTAINER_DV,               // DV (Digital Video)
        CONTAINER_DXA,              // DXA
        CONTAINER_EAC3,             // Enhanced AC-3
        CONTAINER_FLAC,             // FLAC (Free Lossless Audio Codec)
        CONTAINER_FLV,              // FLV (Flash Video)
        CONTAINER_GSM,              // GSM (Global System for Mobile Audio)
        CONTAINER_H261,             // H.261
        CONTAINER_H263,             // H.263
        CONTAINER_H264,             // H.264
        CONTAINER_HLS,              // HLS (Apple HTTP Live Streaming PlayList)
        CONTAINER_MJPEG,            // MJPEG video
        CONTAINER_MOV,              // QuickTime / MOV / MPEG4
        CONTAINER_MP3,              // MP3 (MPEG audio layer 2/3)
        CONTAINER_MPEG2PS,          // MPEG-2 Program Stream
        CONTAINER_MPEG2TS,          // MPEG-2 Transport Stream
        CONTAINER_MPEG4BS,          // MPEG-4 Bitstream
        CONTAINER_OGG,              // Ogg
        CONTAINER_RM,               // RM (RealMedia)
        CONTAINER_SRT,              // SRT (SbuRip subtitle)
        CONTAINER_WAV,              // WAV / WAVE (Waveform Audio)
        CONTAINER_WEBM,             // Matroska / WebM
        CONTAINER_DASH,             // DASH (MPEG-DASH)
        CONTAINER_SMOOTH_STREAM,    // Smooth Streaming
        CONTAINER_MAX = CONTAINER_SMOOTH_STREAM,    // Must be last
    };

    // Minimum size considered for processing.
    enum {
        kMinimumContainerSize = 12
    };

    MediaContainerName DetermineContainer(const uint8_t* buffer,
                                          int buffer_size);
}

#endif //NEWCUT_CONTAINER_NAMES_H
