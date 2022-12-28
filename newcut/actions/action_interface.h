//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_ACTION_INTERFACE_H
#define NEWCUT_ACTION_INTERFACE_H

#include <QObject>
#include "newcut/actions/snapper.h"
#include "newcut/engine/entity_container.h"
#include "newcut/engine/enum_collect.h"
#include "newcut/gui/graphic_view.h"

namespace nc {
    class ActionInterface : public QObject, public Snapper {
    Q_OBJECT
    public:
        ActionInterface(const char* name,
                        EntityContainer& container,
                        GraphicView& graphic_view);

        virtual ~ActionInterface() = default;

        virtual EnumCollect::ActionType Rtti() const;

        void SetName(const char* name);

        QString GetName();

        virtual void Init(int status = 0);

        virtual void MouseMoveEvent(QMouseEvent* event);

        virtual void MousePressEvent(QMouseEvent* event);

        virtual void KeyPressEvent(QKeyEvent* event);

        virtual void KeyReleaseEvent(QKeyEvent* event);

        virtual void Trigger();

    protected:
        QString name_;
    };
}


#endif //NEWCUT_ACTION_INTERFACE_H
