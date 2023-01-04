//
// Created by wangrl2016 on 2022/12/31.
//

#ifndef NEWCUT_TIME_H
#define NEWCUT_TIME_H

#include <cstdint>

namespace base {
    template<class TimeClass>
    class TimeBase {
    public:
        static constexpr int64_t kHoursPerDay = 24;
        static constexpr int64_t kSecondsPerMinute = 60;
        static constexpr int64_t kMinutesPerHour = 60;
        static constexpr int64_t kSecondPerHour =
                kSecondsPerMinute * kMinutesPerHour;
        static constexpr int64_t kMillsecondsPerSecond = 1000;
        static constexpr int64_t kMillsecondsPerDay =
                kMillsecondsPerSecond * kSecondPerHour * kHoursPerDay;
        static constexpr int64_t kMicrosecondPerMillisecond = 1000;
        static constexpr int64_t kMicrosecondPerSecond =
                kMicrosecondPerMillisecond * kMillsecondsPerSecond;
        static constexpr int64_t kMicrosecondPerMinute =
                kMicrosecondPerSecond * kSecondsPerMinute;
    };

    class Time : public TimeBase<Time> {
    };
}


#endif //NEWCUT_TIME_H
