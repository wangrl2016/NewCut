//
// Created by wr on 2022/12/28.
//

#ifndef NEWCUT_ENTITY_H
#define NEWCUT_ENTITY_H

#include <map>
#include <QString>

#include "newcut/engine/undoable.h"
#include "vector.h"
#include "layer.h"
#include "enum_collect.h"
#include "quadratic.h"

namespace nc {
    class EntityContainer;

    // Base class for an entity (line, arc, circle, ...)
    class Entity : public Undoable {
    public:
        Entity(EntityContainer* parent = nullptr);

        virtual ~Entity() = default;

        void Init();

        virtual void InitId();

        virtual Entity* Clone() const = 0;

        virtual void ReParent(EntityContainer* parent) {
            this->parent_ = parent;
        }

        void ResetBorders();

        void MoveBorders(const Vector& offset);

        void ScaleBorders(const Vector& center, const Vector& factor);


        virtual EnumCollect::EntityType Rtti() const {
            return EnumCollect::kEntityUnknown;
        }

        int64_t id() const { return id_; }

        // this method must be overwritten in subclasses and return the
        // number of atomic entities in this entity.
        virtual int count() const = 0;

        // This method must be overwritten in subclasses and return the
        // number of atomic entities in this entity including sub containers.
        virtual int CountDeep() const = 0;

        // Implementations must return the total length of the entity
        // or a negative number if the entity has no length (e.g. a text or hatch).
        virtual double GetLength() const {
            return -1.0;
        }

        EntityContainer* GetParent() const {
            return parent_;
        }

        // Re-parents this entity.
        void set_parent(EntityContainer* p) {
            parent_ = p;
        }

        virtual Quadratic GetQuadratic() const;

        // Line integral for contour area calculation by Green's theorem.
        virtual double ArcLineIntegral() const;

        // Whether the entity type can be trimmed.
        // Currently, trimmable types are: Line, Circle, Arc, Ellipse.
        bool Trimmable() const;

        // Is the entity of type Arc, Circle, or Ellipse.
        // return true for Arc, Circle, or Ellipse.
        virtual bool IsArc() const;

        // Determine the entity is either Arc, Circle, or Line.
        // return true if entity is Arc, Circle, or Line.
        virtual bool IsArcCircleLine() const;

    protected:
        EntityContainer* parent_ = nullptr;
        Vector min_v_;  // minimum coordinates
        Vector max_v_;  // maximum coordinates

        Layer* layer_;  // pointer to layer

        uint64_t id_;   // entity id

        Pen pen_;       // pen(attributes) for this entity

        bool update_enabled_;   // auto updating enabled?

    private:
        std::map<QString, QString> var_list_;
    };
}

#endif //NEWCUT_ENTITY_H
