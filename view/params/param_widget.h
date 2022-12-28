//
// Created by wangrl2016 on 2022/12/22.
//

#ifndef NEWCUT_PARAM_WIDGET_H
#define NEWCUT_PARAM_WIDGET_H

#include <QWidget>
#include <QBasicTimer>

namespace nc {
    class ParamWidget : public QWidget {
    Q_OBJECT
    public:
        explicit ParamWidget(QWidget* parent = nullptr);

        ~ParamWidget() override;

    protected:
        void paintEvent(QPaintEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void showEvent(QShowEvent *event) override;

        void timerEvent(QTimerEvent* event) override;

    private:
        QBasicTimer timer_;
    };
}

#endif //NEWCUT_PARAM_WIDGET_H
