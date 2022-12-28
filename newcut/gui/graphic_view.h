//
// Created by wangrl2016 on 2022/12/28.
//

#ifndef NEWCUT_GRAPHIC_VIEW_H
#define NEWCUT_GRAPHIC_VIEW_H

#include <QWidget>

namespace nc {
    class GraphicView : public QWidget {
    Q_OBJECT
    public:
        GraphicView(QWidget* parent = nullptr, Qt::WindowFlags f = static_cast<QFlag>(0));

        virtual ~GraphicView();

        void CleanUp();
    };
}


#endif //NEWCUT_GRAPHIC_VIEW_H
