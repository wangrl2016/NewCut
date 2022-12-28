//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_ACTION_FILE_NEW_H
#define NEWCUT_ACTION_FILE_NEW_H

#include "newcut/actions/action_interface.h"

namespace nc {
    class ActionFileNew : public ActionInterface {
    Q_OBJECT
    public:
        ActionFileNew(EntityContainer& container,
                      GraphicView& graphic_view);

        void Init(int status = 0) override;

        void Trigger() override;
    };
}


#endif //NEWCUT_ACTION_FILE_NEW_H
