//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PARAMS_GRAPHIC_SCENE_H
#define NEWCUT_PARAMS_GRAPHIC_SCENE_H

#include <QGraphicsScene>

namespace nc {
    class PianoGraphicsScene : public QGraphicsScene {
        Q_OBJECT
    public:
        PianoGraphicsScene(QObject* parent = nullptr);

        ~PianoGraphicsScene() override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    };
}

#endif //NEWCUT_PARAMS_GRAPHIC_SCENE_H
