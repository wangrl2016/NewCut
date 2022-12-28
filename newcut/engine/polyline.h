//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_POLYLINE_H
#define NEWCUT_POLYLINE_H

#include "newcut/engine/flags.h"
#include "newcut/engine/vector.h"
#include "newcut/engine/entity_container.h"


namespace nc {
    struct PolylineData : public Flags {
        PolylineData();

        ~PolylineData() = default;

        PolylineData(const Vector& start_point,
                     const Vector& end_point,
                     bool closed);

        Vector start_point_;
        Vector end_point_;
    };

    std::ostream& operator<<(std::ostream& os, const PolylineData& pd);

    // Class for a poly line entity (lots of connected lines and arcs.
    class Polyline : public EntityContainer {

    };
}

#endif //NEWCUT_POLYLINE_H
