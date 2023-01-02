//
// Created by wangrl2016 on 2023/1/2.
//

#ifndef NEWCUT_BIT_H
#define NEWCUT_BIT_H

#include <type_traits>

namespace base {
    // Returns true if |value| is power of 2.
    template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
    constexpr bool IsPowerOfTwo(T value) {
        // Only positive integers with a single bit set are powers of two. If only one
        // bit is set in x (e.g. 0b00000100000000) then |x-1| will have that bit set
        // to zero and all bits to its right set to 1 (e.g. 0b00000011111111). Hence
        // |x & (x-1)| is 0 if x is a power of two.
        return value > 0 && (value & (value - 1)) == 0;
    }
}

#endif //NEWCUT_BIT_H
