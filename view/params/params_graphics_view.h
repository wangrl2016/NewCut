//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PARAMS_GRAPHICS_VIEW_H
#define NEWCUT_PARAMS_GRAPHICS_VIEW_H

#include <QGraphicsView>

namespace nc {
    class ParamsGraphicsView : public QGraphicsView {
        Q_OBJECT
    public:
        ParamsGraphicsView(QGraphicsScene* scene,
                           QWidget* parent = nullptr);

        ~ParamsGraphicsView() override;
    };
}

#endif //NEWCUT_PARAMS_GRAPHICS_VIEW_H
