//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_TRACK_GRAPHICS_VIEW_H
#define NEWCUT_TRACK_GRAPHICS_VIEW_H

#include <QGraphicsView>

namespace nc {
    class TrackGraphicsView : public QGraphicsView {
    Q_OBJECT
    public:
        TrackGraphicsView(QGraphicsScene* scene,
                          QWidget* widget = nullptr);

        ~TrackGraphicsView() override;
    };
}

#endif //NEWCUT_TRACK_GRAPHICS_VIEW_H
