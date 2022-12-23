//
// Created by wangrl2016 on 2022/12/23.
//

#ifndef NEWCUT_ARTHUR_WIDGET_H
#define NEWCUT_ARTHUR_WIDGET_H

#include <QWidget>

namespace nc {
    class ArthurFrame : public QWidget {
    Q_OBJECT
    public:
        ArthurFrame(QWidget* parent = nullptr);

        virtual void Paint(QPainter *) {}

        bool prefer_image() const {
            return prefer_image_;
        }

    protected:
        void paintEvent(QPaintEvent* event) override;

        void resizeEvent(QResizeEvent* event) override;

        QPixmap tile_;

        bool prefer_image_ = false;
    };
}

#endif //NEWCUT_ARTHUR_WIDGET_H
