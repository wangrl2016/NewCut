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

    private:
        EnumCollect::ActionType order_type_;

    };
}

#endif //NEWCUT_ACTION_HANDLER_H
