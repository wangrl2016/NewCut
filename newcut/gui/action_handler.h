//
// Created by wangrl2016 on 2022/12/29.
//

#ifndef NEWCUT_ACTION_HANDLER_H
#define NEWCUT_ACTION_HANDLER_H

#include <QObject>
#include "newcut/actions/action_interface.h"

namespace nc {
    // This class can trigger actions (from menus, buttons, ...)
    class ActionHandler : public QObject {
        Q_OBJECT
    public:
        ActionHandler(QObject* parent);

        virtual ~ActionHandler() = default;

        ActionInterface* GetCurrentAction();

        ActionInterface* SetCurrentAction(EnumCollect::ActionType id);

        // Kills all running selection actions. Called when a selection action
        // is launched to reduce confusion.
        void KillSelectActions();

        void KillAllActions();

        bool KeyCode(const QString& code);

    public slots:
        void SlotFileNewTemplate();

        void SlotFileOpen();

        void SlotFileSaveAs();

        void SlotFileExportMakerCam();

        void SlotZoomIn();

        void SlotZoomOut();

        void SlotZoomAuto();

        void SlotZoomWindow();

        void SlotZoomPan();

        void SlotZoomPrevious();

        void SlotZoomReDraw();

        void SlotTooRegenerateDimensions();

        void SlotEditKillAllActions();

        void SlotEditUndo();

        void SlotEditRedo();

        void SlotEditCut();

        void SlotEditCopy();

        void SlotEditPaste();

        void SlotOrderBottom();

        void SlotOrderLower();

        void SlotOrderRaise();

        void SlotOrderTop();


    private:
        EnumCollect::ActionType order_type_;

    };
}

#endif //NEWCUT_ACTION_HANDLER_H
