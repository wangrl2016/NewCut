//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_TRACK_GRAPHICS_SCENE_H
#define NEWCUT_TRACK_GRAPHICS_SCENE_H

#include <QGraphicsScene>

namespace nc {
    class TrackGraphicsScene : public QGraphicsScene {
    Q_OBJECT
    public:
        TrackGraphicsScene(QObject* parent = nullptr);

        ~TrackGraphicsScene() override;
    };
}

#endif //NEWCUT_TRACK_GRAPHICS_SCENE_H
