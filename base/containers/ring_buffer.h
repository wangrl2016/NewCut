//
// Created by wangrl2016 on 2022/12/29.
//

#ifndef NEWCUT_RING_BUFFER_H
#define NEWCUT_RING_BUFFER_H

#include <cstdlib>

namespace base {
    template<typename T, size_t kSize>
    class RingBuffer {
    public:
        RingBuffer() : current_index_(0) {}

        RingBuffer(const RingBuffer&) = delete;

        RingBuffer& operator=(const RingBuffer&) = delete;

        size_t BufferSize() const { return kSize; }

        size_t CurrentIndex() const { return current_index_; }

        bool IsFilledIndex(size_t n) const {
            return IsFilledIndexByBufferIndex(BufferIndex(n));
        }

        // Returns the element at index |n| (% |kSize|).
        //
        // n = 0 return the oldest value and
        // n = bufferSize() - 1 returns the most recent value.
        const T& ReadBuffer(size_t n) const {
            const size_t buffer_index = BufferIndex(n);
            CHECK(IsFilledIndexByBufferIndex(buffer_index));
            return buffer_[buffer_index];
        }

        T* MutableReadBuffer(size_t n) {
            const size_t buffer_index = BufferIndex(n);
            CHECK(IsFilledIndexByBufferIndex(buffer_index));
            return &buffer_[buffer_index];
        }

        void SaveToBuffer(const T& value) {
            buffer_[BufferIndex(0)] = value;
            current_index_++;
        }

        void Clear() { current_index_ = 0; }


    private:
        inline size_t BufferIndex(size_t n) const {
            return (current_index_ + n) % kSize;
        }

        // This specialization of |IsFilledIndex| is a micro-optimization that enables
        // us to do e.g. `CHECK(IsFilledIndex(n))` without calling |BufferIndex|
        // twice. Since |BufferIndex| involves a % operation, it's not quite free at a
        // microscale.
        inline bool IsFilledIndexByBufferIndex(size_t buffer_index) const {
            return buffer_index < current_index_;
        }

        T buffer_[kSize];
        size_t current_index_;
    };
}

#endif //NEWCUT_RING_BUFFER_H
