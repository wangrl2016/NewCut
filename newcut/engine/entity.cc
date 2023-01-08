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

    void Entity::ResetBorders() {

    }

    void Entity::MoveBorders(const Vector& offset) {

    }

    void Entity::ScaleBorders(const Vector& center, const Vector& factor) {

    }

    Quadratic Entity::GetQuadratic() const {
        return Quadratic();
    }

    double Entity::ArcLineIntegral() const {
        return 0.0;
    }

    bool Entity::Trimmable() const {
        return true;
    }

    bool Entity::IsArc() const {
        return true;
    }

    bool Entity::IsArcCircleLine() const {
        return true;
    }
}
