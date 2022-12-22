//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PIANO_GRAPHICS_VIEW_H
#define NEWCUT_PIANO_GRAPHICS_VIEW_H

#include <QGraphicsView>

namespace nc {
    class PianoGraphicsView : public QGraphicsView {
        Q_OBJECT
    public:
        PianoGraphicsView(QGraphicsScene* scene,
                          QWidget* parent = nullptr);

        ~PianoGraphicsView() override;
    };
}

#endif //NEWCUT_PIANO_GRAPHICS_VIEW_H
