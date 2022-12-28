//
// Created by wr on 2022/12/28.
//

#include "newcut/actions/action_file_new.h"

namespace nc {
    ActionFileNew::ActionFileNew(nc::EntityContainer& container, nc::GraphicView& graphic_view) :
            ActionInterface("File New", container, graphic_view) {

    }

    void ActionFileNew::Init(int status) {
        ActionInterface::Init(status);
        Trigger();
    }

    void ActionFileNew::Trigger() {

    }
}