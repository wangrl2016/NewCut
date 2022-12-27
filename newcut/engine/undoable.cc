//
// Created by wr on 2022/12/28.
//

#include "newcut/engine/undoable.h"

namespace nc {
    // The undoable thing gets activated if it was undone and
    // deactivated otherwise.
    void Undoable::ChangeUndoState() {
        ToggleFlag(kFlagUndone);
        UndoStateChanged(IsUndone());
    }

    void Undoable::SetUndoState(bool undone) {
        if (undone) {
            SetFlag(kFlagUndone);
        } else {
            DelFlag(kFlagUndone);
        }
        UndoStateChanged(IsUndone());
    }

    bool Undoable::IsUndone() const {
        return false;
    }
}