//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_ACTION_INTERFACE_H
#define NEWCUT_ACTION_INTERFACE_H

#include <QObject>
#include "newcut/actions/snapper.h"
#include "newcut/engine/entity_container.h"
#include "newcut/gui/graphic_view.h"

namespace nc {
    class ActionInterface : public QObject, public Snapper {
    Q_OBJECT
    public:
        ActionInterface(const char* name,
                        EntityContainer& container,
                        GraphicView& graphic_view);

        virtual ~ActionInterface() = default;

    protected:
        QString name_;
    };
}


#endif //NEWCUT_ACTION_INTERFACE_H
