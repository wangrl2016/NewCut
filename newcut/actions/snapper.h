//
// Created by wangrl2016 on 2022/12/27.
//

#ifndef NEWCUT_SNAPPER_H
#define NEWCUT_SNAPPER_H

#include <cstdint>
#include <initializer_list>
#include "newcut/engine/enum_collect.h"
#include "newcut/engine/entity_container.h"
#include "newcut/gui/graphic_view.h"

namespace nc {
    // This class holds information on how to snap the mouse.
    class SnapMode {
        // SnapModes for SnapMode to Int conversion and vice versa.
        // The conversion is only used for save/restore of active snap modes in application settings.
        // Don't change existing mode order, because this will mess up settings on upgrades.
        enum SnapModes {
            KSnapIntersection       = 1 << 0,
            kSnapOnEntity           = 1 << 1,
            kSnapCenter             = 1 << 2,
            kSnapDistance           = 1 << 3,
            kSnapMiddle             = 1 << 4,
            kSnapEndpoint           = 1 << 5,
            kSnapGrid               = 1 << 6,
            kSnapFree               = 1 << 7,
            kRestrictHorizontal     = 1 << 8,
            kRestrictVertical       = 1 << 9,
            kRestrictOrthogonal = kRestrictHorizontal | kRestrictVertical,
            kSnapAngle              = 1 << 10
        };

    public:
        bool snap_intersection_ = false;
        bool snap_on_entity_ = false;
        bool snap_center_ = false;
        bool snap_distance_ = false;
        bool snap_middle_ = false;
        bool snap_endpoint_ = false;
        bool snap_grid_ = false;
        bool snap_free_ = false;
        bool snap_angle_ = false;

        // The restriction on the free snap
        EnumCollect::SnapRestriction restriction_ = EnumCollect::kRestrictNothing;

        // The distance to snap before defaulting to free snapping.
        double distance_ = 5.0;

        // Disable all snapping.
        // This effectively puts the object into free snap mode.
        SnapMode const& Clear();

        bool operator==(SnapMode const& rhs) const;

        // convert to int, to save settings
        static uint32_t ToInt(const SnapMode& s);

        static SnapMode FromInt(uint32_t i);
    };

    typedef std::initializer_list<EnumCollect::EntityType> EntityTypeList;

    // This class is used for snapping functions in a graphic view.
    // Actions are usually derived from this base class if they need
    // to catch entities or snap to coordinate. Use the methods to
    // retrieve a graphic coordinate from a mouse coordinate.
    class Snapper {
    public:
        Snapper() = delete;

        Snapper(EntityContainer& container, GraphicView& graphic_view);

    protected:
        EntityContainer* container_;
        GraphicView* graphic_view_;
    };
}


#endif //NEWCUT_SNAPPER_H
