//
// Created by wangrl2016 on 2022/12/27.
//

#include "newcut/actions/snapper.h"

namespace nc {
    Snapper::Snapper(nc::EntityContainer& container, nc::GraphicView& graphic_view)
            : container_(&container), graphic_view_(&graphic_view) {

    }
}