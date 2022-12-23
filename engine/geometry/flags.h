//
// Created by wr on 2022/12/22.
//

#ifndef NEWCUT_FLAGS_H
#define NEWCUT_FLAGS_H

#include <cstdint>

namespace engine {
    enum UndoableType {
        kUndoableUnknown,       // Unknown undoable
        kUndoableEntity,        // Entity
        kUndoableLayer          // Layer
    };

    enum Flag {
        kFlagUndone         = 1 << 0,       // flag for undoable
        kFlagVisible        = 1 << 1,       // entity visibility
        kFlagByLayer        = 1 << 2,       // entity attribute (e.g. color) is defined by layer
        kFlagByBlock        = 1 << 3,       // entity attribute (e.g. color) is defined by block
        kFlagFrozen         = 1 << 4,       // layer frozen
        kFlagDefFrozen      = 1 << 5,       // layer frozen by default
        kFlagLocked         = 1 << 6,       // layer locked
        kFlagInvalid        = 1 << 7,       // used for invalid pens
        kFlagSelected       = 1 << 8,       // entity in current selection
    };

    struct Flags {
    public:
        explicit Flags(uint32_t f = 0);

        virtual ~Flags() = default;

        uint32_t flags() const;

        void ResetFlags();

        void set_flags(uint32_t f);

        void SetFlag(uint32_t f);

        void DelFlag(uint32_t f);

        void ToggleFlag(uint32_t f);

        bool GetFlag(uint32_t f) const;

    private:
        uint32_t flags_ = 0;
    };
}


#endif //NEWCUT_FLAGS_H
