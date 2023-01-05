//
// Created by wangrl2016 on 2022/12/29.
//

#include <glog/logging.h>
#include "newcut/gui/action_handler.h"

namespace nc {
    ActionHandler::ActionHandler(QObject* parent)
            : QObject(parent) {
        LOG(INFO) << __FUNCTION__ << " OK";
    }

    void ActionHandler::SlotFileNewTemplate() {

    }

    void ActionHandler::SlotFileOpen() {

    }

    void ActionHandler::SlotFileSaveAs() {

    }

    void ActionHandler::SlotFileExportMakerCam() {

    }

    void ActionHandler::SlotZoomIn() {

    }

    void ActionHandler::SlotZoomOut() {

    }

    void ActionHandler::SlotZoomAuto() {

    }

    void ActionHandler::SlotZoomWindow() {

    }

    void ActionHandler::SlotZoomPan() {

    }

    void ActionHandler::SlotZoomPrevious() {

    }

    void ActionHandler::SlotZoomReDraw() {

    }

    void ActionHandler::SlotTooRegenerateDimensions() {

    }

    void ActionHandler::SlotEditKillAllActions() {

    }

    void ActionHandler::SlotEditUndo() {

    }

    void ActionHandler::SlotEditRedo() {

    }

    void ActionHandler::SlotEditCut() {

    }

    void ActionHandler::SlotEditCopy() {

    }

    void ActionHandler::SlotEditPaste() {

    }

    void ActionHandler::SlotOrderBottom() {

    }

    void ActionHandler::SlotOrderLower() {

    }

    void ActionHandler::SlotOrderRaise() {

    }

    void ActionHandler::SlotOrderTop() {

    }


}
