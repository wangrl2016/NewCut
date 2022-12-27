//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_UNDOABLE_H
#define NEWCUT_UNDOABLE_H

#include "newcut/engine/flags.h"

namespace nc {
    class Undoable : public Flags {
        // Runtime type identification for undoables.
        virtual UndoableType UndoRtti() const {
            return kUndoableUnknown;
        }

        void ChangeUndoState();

        void SetUndoState(bool undone);

        bool IsUndone() const;

        // Can be overritten by the implementing class to be notified
        // when the undo state change (the undoable becomes visible/invisible)
        virtual void UndoStateChanged(bool undone) = 0;
    };
}


#endif //NEWCUT_UNDOABLE_H
