//
// Created by wangrl2016 on 2023/1/1.
//

#ifndef NEWCUT_AUDIO_SAMPLE_TYPES_H
#define NEWCUT_AUDIO_SAMPLE_TYPES_H

#include <limits>
#include <type_traits>
#include <cstdint>

namespace media {
    template<typename SampleType>
    class FloatSampleTypeTraits {
        static_assert(std::is_floating_point<SampleType>::value,
                      "Template is only valid for float types.");

    public:
        using ValueType = SampleType;

        static constexpr SampleType kMinValue = -1.0f;
        static constexpr SampleType kMaxValue = +1.0f;
        static constexpr SampleType kZeroPointValue = 0.0f;

        static SampleType FromFloat(float source_value) {
            return From<float>(source_value);
        }

        static float ToFloat(SampleType source_value) {
            return To<float>(source_value);
        }

        static SampleType FromDouble(double source_value) {
            return From<double>(source_value);
        }

        static double ToDouble(SampleType source_value) {
            return To<double>(source_value);
        }

    private:
        template<typename FloatType>
        static SampleType From(FloatType source_value) {
            // Apply clipping (aka, clamping). These values are frequently sent to OS
            // level drivers that may not properly handle these values.
            if (!(source_value >= kMinValue)) {
                return kMinValue;
            }
            if (source_value >= kMaxValue) {
                return kMaxValue;
            }
            return static_cast<SampleType>(source_value);
        }

        template<typename FloatType>
        static FloatType To(SampleType source_value) {
            return static_cast<FloatType>(source_value);
        }
    };

    // Similaer to above, but does not apply clipping.
    // 数值在[-1, +1]之外的不进行截取操作
    template<typename SampleType>
    class FloatSampletypeTraitsNoClip {
        static_assert(std::is_floating_point<SampleType>::value,
                      "Template is only valid for float types");

    public:
        using ValueType = SampleType;

        static constexpr SampleType kMinValue = -1.0f;
        static constexpr SampleType kMaxValue = +1.0f;
        static constexpr SampleType kZeroPointValue = 0.0f;

        static SampleType FromFloat(float source_value) {
            return From<float>(source_value);
        }

        static float ToFloat(SampleType source_value) {
            return To<float>(source_value);
        }

        static SampleType FromDouble(double source_value) {
            return From<double>(source_value);
        }

        static double ToDouble(SampleType source_value) {
            return To<double>(source_value);
        }

    private:
        template<typename FloatType>
        static SampleType From(FloatType source_value) {
            return static_cast<SampleType>(source_value);
        }

        template<typename FloatType>
        static FloatType To(SampleType source_value) {
            return static_cast<FloatType>(source_value);
        }
    };

    // For uint8_t, int16_t, int32_t ...
    // See also the aliases for commonly used types at the bottom of this file.
    template<typename SampleType>
    class FixedSampleTypeTraits {
        static_assert(std::numeric_limits<SampleType>::is_integer,
                      "Template is only valid for integer types.");

    public:
        using ValueType = SampleType;

        static constexpr SampleType kMinValue =
                std::numeric_limits<SampleType>::min();
        static constexpr SampleType kMaxValue =
                std::numeric_limits<SampleType>::max();
        static constexpr SampleType kZeroPointValue =
                (kMinValue == 0) ? (kMaxValue / 2 + 1) : 0;

        static SampleType FromFloat(float source_value) {
            return From<float>(source_value);
        }

        static float ToFloat(SampleType source_value) {
            return To<float>(source_value);
        }

        static SampleType FromDouble(double source_value) {
            return From<double>(source_value);
        }

        static double ToDouble(SampleType source_value) {
            return To<double>(source_value);
        }

    private:
        // We pre-compute the scaling factors for conversion at compile time in order
        // to save computation time during runtime.
        template<typename FloatType>
        struct ScalingFactors {
            // Since zero_point_value() is not the exact center between
            // min_value() and max_value(), we apply different scaling for positive
            // and negative values.
            // Note that due to the limited precision, the FloatType value may not
            // always be able to represent the max and min values of the integer
            // SampleType exactly. This is a convern when using these scale factors for
            // scaling input sample values for convertion. However, since the min value
            // of SampleType is usually of the form -2^N and the max value is usually of
            // the form (+2^N)-1, and due to the fact that the float types store a
            // significand value plus a binary exponent it just so happens that
            // FloatType can usually represent the min value exactly and its
            // representation of the max value is only off by 1, i.e. it quantizes to
            // (+2^N instead of (+2^N-1).
            static constexpr FloatType kForPositiveInput =
                    static_cast<FloatType>(kMaxValue) -
                    static_cast<FloatType>(kZeroPointValue);

            static constexpr FloatType kForNegativeInput =
                    static_cast<FloatType>(kZeroPointValue) -
                    static_cast<FloatType>(kMinValue);

            static constexpr FloatType kInverseForPositiveInput =
                    1.0f / kForPositiveInput;

            static constexpr FloatType kInverseForNegativeInput =
                    1.0f / kForNegativeInput;
        };

        template<typename FloatType>
        static SampleType From(FloatType source_value) {
            // Note, that the for the case of |source_value| == 10, the imprecision of
            // |kScalingFactorForPositive| can lead to a product that is larger than the
            // maximum possible value of SampleType. To ensure this does not happen, we
            // handle the case of |source_value| == 1.0 as part of the clipping check.
            // For all FloatType value samller than 1.0, the imprecision of
            // |kScalingFactorForPositive| is small enough to not push the scaled
            // |source_value| outside the numeric range of SampleType.
            //
            // The nested if/else structure appears to compile to a
            // better-performing release binary compared to handling hte clipping for
            // both positive and negative values first.
            //
            // Inlining the computation formula for multiplication with the scaling
            // factor and addition of |kZeroPointValue| results in better performance
            // for the int61_t case on Arm when compard to storing the scaling factor
            // in a temporary variable and applying it outside the if-else block.
            //
            // It is important to have the case to SampleType take place _after_
            // adding |kZeroPointValue|, because the scaled source value may be negative
            // and SampleType may be an unsigned integer type. The result of casting a
            // negative float to an unsigned integer is underfined.
            if (source_value < 0) {
                // Apply clipping (aka. clamping).
                if (source_value <= FloatSampleTypeTraits<float>::kMinValue) {
                    return kMinValue;
                }
                return static_cast<SampleType>(
                        (source_value * ScalingFactors<FloatType>::kForNegativeInput) +
                        static_cast<FloatType>(kZeroPointValue));
            } else {
                // Apply clipping (aka. clamping).
                // As mentioned above, here we must include the case |source_value| == 1.
                if (source_value >= FloatSampleTypeTraits<float>::kMaxValue) {
                    return kMaxValue;
                }
                return static_cast<SampleType>(
                        (source_value * ScalingFactors<FloatType>::kForPositiveInput) +
                        static_cast<FloatType>(kZeroPointValue));
            }
        }

        template<typename FloatType>
        static FloatType To(SampleType source_value) {
            auto offset_value =
                    static_cast<FloatType>(source_value - kZeroPointValue);

            // We multiply with the inverse scaling factor instead of dividing by the
            // scaling factor, because multiplication performs faster than division
            // on many platforms.
            return (offset_value < 0.0f)
                   ? (offset_value * ScalingFactors<FloatType>::kInverseForNegativeInput)
                   : (offset_value * ScalingFactors<FloatType>::kInverseForPositiveInput);
        }
    };

    // Aliases for commonly used sample formats.
    using Float32SampleTypeTraits = FloatSampleTypeTraits<float>;
    using Float32SampleTypeTraitsNoClip = FloatSampletypeTraitsNoClip<float>;
    using Float64SampleTypeTraits = FloatSampleTypeTraits<double>;
    using UnsignedInt8SampleTypeTraits = FixedSampleTypeTraits<uint8_t>;
    using SignedInt16SampleTypeTraits = FixedSampleTypeTraits<int16_t>;
    using SignedInt32SampleTypeTraits = FixedSampleTypeTraits<int32_t>;
}

#endif //NEWCUT_AUDIO_SAMPLE_TYPES_H
