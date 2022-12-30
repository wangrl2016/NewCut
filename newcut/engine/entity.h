//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_ENTITY_H
#define NEWCUT_ENTITY_H

#include <map>
#include <QString>

#include "newcut/engine/undoable.h"
#include "entity_container.h"
#include "vector.h"

namespace nc {
    class Entity : public Undoable {
    public:
        Entity(EntityContainer* parent = nullptr);

        virtual ~Entity() = default;

        void Init();

        virtual void InitId();

        virtual Entity* Clone() const = 0;

        virtual void ReParent(EntityContainer* parent) {

        }

    protected:
        EntityContainer* parent_ = nullptr;
        Vector min_v_;  // minimum coordinates
        Vector max_v_;  // maximum coordinates

        // Layer* layer_;  // pointer to layer

        uint64_t id_;   // entity id

        // Pen pen_;       // pen(attributes) for this entity

        bool update_enabled_;   // auto updating enabled?

    private:
        std::map<QString, QString> var_list_;
    };
}

#endif //NEWCUT_ENTITY_H
