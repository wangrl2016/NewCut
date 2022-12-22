//
// Created by admin on 2022/12/21.
//

#include <glog/logging.h>
#include "media/base/sample_format.h"

namespace media {
    int SampleFormatToBytesPerChannel(SampleFormat sample_format) {
        switch (sample_format) {
            case kUnknownSampleFormat:
                return 0;
            case kSampleFormatU8:
            case kSampleFormatPlanarU8:
            case kSampleFormatAc3:
                return 1;
            case kSampleFormatS16:
            case kSampleFormatPlanarS16:
                return 2;
            case kSampleFormatS24:
            case kSampleFormatS32:
            case kSampleFormatF32:
            case kSampleFormatPlanarF32:
            case kSampleFormatPlanarS32:
                return 4;
        }

        LOG(ERROR) << "Invalid sample format provied: " << sample_format;
        return 0;
    }

    int SampleFormatToBitsPerChannel(SampleFormat sample_format) {
        return SampleFormatToBytesPerChannel(sample_format) * 8;
    }

    const char* SampleFormatToString(SampleFormat sample_format) {
        switch (sample_format) {
            case kUnknownSampleFormat:
                return "Unknown sample format";
            case kSampleFormatU8:
                return "Unsigned 8-bit with bias of 128";
            case kSampleFormatS16:
                return "Signed 16-bit";
            case kSampleFormatS24:
                return "Signed 24-bit";
            case kSampleFormatS32:
                return "Signed 32-bit";
            case kSampleFormatF32:
                return "Float 32-bit";
            case kSampleFormatPlanarU8:
                return "Unsigned 8-bit with bias 128 planar";
            case kSampleFormatPlanarS16:
                return "Signed 16-bit planar";
            case kSampleFormatPlanarF32:
                return "Float 32-bit planar";
            case kSampleFormatPlanarS32:
                return "Signed 32-bit planar";
            case kSampleFormatAc3:
                return "Compressed AC3 bitstream";
        }
        LOG(ERROR) << "Invalid sample format provided: " << sample_format;
        return "";
    }

    bool IsPlanar(SampleFormat sample_format) {
        switch (sample_format) {
            case kSampleFormatPlanarU8:
            case kSampleFormatPlanarS16:
            case kSampleFormatPlanarS32:
            case kSampleFormatPlanarF32:
                return true;
            case kUnknownSampleFormat:
            case kSampleFormatU8:
            case kSampleFormatS16:
            case kSampleFormatS24:
            case kSampleFormatS32:
            case kSampleFormatF32:
            case kSampleFormatAc3:
                return false;
        }

        LOG(ERROR) << "Invalid sample format provided: " << sample_format;
        return false;
    }

    bool IsInterleaved(SampleFormat sample_format) {
        switch (sample_format) {
            case kSampleFormatU8:
            case kSampleFormatS16:
            case kSampleFormatS24:
            case kSampleFormatS32:
            case kSampleFormatF32:
            case kSampleFormatAc3:
                return true;
            case kUnknownSampleFormat:
            case kSampleFormatPlanarU8:
            case kSampleFormatPlanarS16:
            case kSampleFormatPlanarF32:
            case kSampleFormatPlanarS32:
                return false;
        }
        LOG(ERROR) << "Invalid sample format provided: " << sample_format;
        return false;
    }
}
