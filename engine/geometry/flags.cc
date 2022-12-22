//
// Created by wr on 2022/12/22.
//

#include "engine/geometry/flags.h"

namespace engine {
    Flags::Flags(uint32_t f) : flags_(f) {}

    uint32_t Flags::flags() const {
        return flags_;
    }

    void Flags::ResetFlags() {
        flags_ = 0;
    }

    void Flags::set_flags(uint32_t f) {
        flags_ = f;
    }

    void Flags::SetFlag(uint32_t f) {
        flags_ |= f;
    }

    void Flags::DelFlag(uint32_t f) {
        flags_ &= ~f;
    }

    void Flags::ToggleFlag(uint32_t f) {
        // 按位异或
        flags_ ^= f;
    }

    bool Flags::GetFlag(uint32_t f) const {
        return flags_ & f;
    }
}
