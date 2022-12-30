//
// Created by wangrl2016 on 2022/12/28.
//

#include "newcut/engine/entity.h"

namespace nc {
    // Default constructor
    //
    // The parent entity of this entity. E.g. a line might
    // have a graphic entity of a polyline entity as parent.
    Entity::Entity(nc::EntityContainer* parent) {
        this->parent_ = parent;
        Init();
    }

    void Entity::Init() {

    }

    void Entity::InitId() {

    }

    Entity* Entity::Clone() const {
        return nullptr;
    }
}