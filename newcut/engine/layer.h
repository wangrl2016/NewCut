//
// Created by wangrl2016 on 2022/12/30.
//

#ifndef NEWCUT_LAYER_H
#define NEWCUT_LAYER_H

#include <QString>
#include "newcut/engine/pen.h"

namespace nc {
    struct LayerData {
        LayerData() = default;

        LayerData(const QString& name,
                  const Pen& pen,
                  bool frozen,
                  bool locked);
    };

    class Layer {
    public:
        explicit Layer(const QString& name);

        Layer* Clone() const;

        void SetName(const QString& name);
    };
}

#endif //NEWCUT_LAYER_H
