//
// Created by admin on 2022/12/21.
//

#ifndef NEWCUT_SAMPLE_FORMAT_H
#define NEWCUT_SAMPLE_FORMAT_H

namespace media {
    enum SampleFormat {
        // These values are histogram over time; do not change their ordinal
        // values.  When deleting a sample format replace it with a dummy value; when
        // adding a sample format, do so at the bottom before kSampleFormatMax, and
        // update the value of kSampleFormatMax.
        kUnknownSampleFormat = 0,
        kSampleFormatU8,                // Unsigned 8-bit w/ bias of 128.
        kSampleFormatS16,               // Signed 16-bit.
        kSampleFormatS32,               // Signed 32-bit.
        kSampleFormatF32,               // Float 32-bit.
        kSampleFormatPlanarS16,         // Signed 16-bit planar.
        kSampleFormatPlanarF32,         // Float 32-bit planar.
        kSampleFormatPlanarS32,         // Signed 32-bit planar
        kSampleFormatS24,               // Signed 24-bit.
        kSampleFormatAc3,               // Compressed AC3 bitstream.
        kSampleFormatPlanarU8,          // Unsigned 8-bit w/ bias of 128 planar.

        // Must always be equal to the largest value ever logged.
        kSampleFormatMax = kSampleFormatPlanarU8,
    };

    // Returns the number of bytes used per channel for the specified
    // |sample_format|.
    int SampleFormatToBytesPerChannel(SampleFormat sample_format);

    int SampleFormatToBitsPerChannel(SampleFormat sample_format);

    // Returns the name of the sample format as a string.
    const char* SampleFormatToString(SampleFormat sample_format);

    // Return true if |sample_format| is planar, false otherwise.
    bool IsPlanar(SampleFormat sample_format);

    bool IsInterleaved(SampleFormat sample_format);
}

#endif //NEWCUT_SAMPLE_FORMAT_H
