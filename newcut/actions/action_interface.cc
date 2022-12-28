//
// Created by wangrl2016 on 2022/12/28.
//

#include "newcut/actions/action_interface.h"

namespace nc {
    ActionInterface::ActionInterface(const char* name, nc::EntityContainer& container,
                                     nc::GraphicView& graphic_view)
                                     : Snapper(container, graphic_view) {}

    EnumCollect::ActionType ActionInterface::Rtti() const {
        return EnumCollect::kActionDefault;
    }

    void ActionInterface::SetName(const char* name) {

    }

    QString ActionInterface::GetName() {
        return {};
    }

    void ActionInterface::Init(int status) {

    }

    void ActionInterface::MouseMoveEvent(QMouseEvent* event) {

    }

    void ActionInterface::MousePressEvent(QMouseEvent* event) {

    }

    void ActionInterface::KeyPressEvent(QKeyEvent* event) {

    }

    void ActionInterface::KeyReleaseEvent(QKeyEvent* event) {

    }

    void ActionInterface::Trigger() {

    }
}