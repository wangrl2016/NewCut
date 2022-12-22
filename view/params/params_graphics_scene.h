//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PARAMS_GRAPHIC_SCENE_H
#define NEWCUT_PARAMS_GRAPHIC_SCENE_H

#include <QGraphicsScene>

namespace nc {
    class ParamsGraphicsScene : public QGraphicsScene {
        Q_OBJECT
    public:
        ParamsGraphicsScene(QObject* parent = nullptr);

        ~ParamsGraphicsScene() override;
    };
}

#endif //NEWCUT_PARAMS_GRAPHIC_SCENE_H
