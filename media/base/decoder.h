//
// Created by wangrl2016 on 2023/1/10.
//

#ifndef NEWCUT_DECODER_H
#define NEWCUT_DECODER_H

#include <string>

namespace media {
    // List of known AudioDecoder implementations; recorded to UKM, always add new
    // values to the end and do not reorder or delete values from this list.
    enum class AudioDecoderType : int {
        kUnknown        = 0,    // Decoder name string is not recognized or n/a.
        kFFmpeg         = 1,    // FFmpegAudioDecoder
        kMojo           = 2,    // MojoAudioDecoder
        kDecrypting     = 3,    // DecryptingAudioDecoder
        kMediaCodec     = 4,    // MediaCodecAudioDecoder (Android)
        kBroker         = 5,    // AudioDecoderBroker
        kTesting        = 6,    // Never send this to UKM, for tests only.
        kAudioToolbox   = 7,    // AudioToolbox (macOS)
        kMediaFoundation= 8,    // MediaFoundationAudioDecoder
        kPassthroughDTS = 9,    // Passthrough DTS audio

        // Keep this at the end and equal to the last entry
        kMaxValue       = kPassthroughDTS,
    };

    // List of known VideoDecoder implementations; recorded to UKM, always add new
    // values to the end and do not reorder or delete values from this list.
    enum class VideoDecoderType : int {
        kUnknown        = 0,    // Decoder name string is not recognized or n/a.
        kFFmpeg         = 2,    // FFmpegVideoDecoder
        kVpx            = 3,    // VpxVideoDecoder
        kAom            = 4,    // AomVideoDecoder
        kMojo           = 5,    // MojoVideoDecoder
        kDecrypting     = 6,    // DecryptingVideoDecoder
        kDav1d          = 7,    // Dav1dVideoDecoder
        kTesting        = 17,   // Never send this to UKM, for tess only

        // Keep this at the end and equal to the last entry.
        kMaxValue       = kTesting,
    };

    std::string GetDecoderName(AudioDecoderType type);

    std::string GetDecoderName(VideoDecoderType type);

    std::ostream& operator<<(std::ostream& out, AudioDecoderType type);

    std::ostream& operator<<(std::ostream& out, VideoDecoderType type);

    class Decoder {
    public:
        virtual ~Decoder();

        // Returns true if the implementation is expected to be implemented by the
        // platform. The value should be available immediately after construction and
        // should not change within the lifetime of a decoder instance.
        virtual bool IsPlatformDecoder() const;

        virtual bool SupportsDecryption() const;

    protected:
        Decoder();
    };
}


#endif //NEWCUT_DECODER_H
