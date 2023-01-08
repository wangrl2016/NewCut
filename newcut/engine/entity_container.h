//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_ENTITY_CONTAINER_H
#define NEWCUT_ENTITY_CONTAINER_H

#include <QList>
#include "newcut/engine/entity.h"

namespace nc {

    // Class representing a tree of entities.
    // Typical entity container are graphics, polylines, groups, texts, ...
    class EntityContainer : public Entity {
        typedef Entity* ValueType;

    public:

    protected:
        QList<Entity*> entities_;
    };
}


#endif //NEWCUT_ENTITY_CONTAINER_H
