//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PARAMS_WIDGET_H
#define NEWCUT_PARAMS_WIDGET_H

#include <QWidget>

namespace nc {
    class ParamsWidget : public QWidget {
    Q_OBJECT
    public:
        ParamsWidget(QWidget* parent = nullptr);

        ~ParamsWidget() override;

    protected:
        void paintEvent(QPaintEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void showEvent(QShowEvent *event) override;
    };
}

#endif //NEWCUT_PARAMS_WIDGET_H
