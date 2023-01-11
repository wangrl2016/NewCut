//
// Created by wangrl2016 on 2023/1/11.
//

#ifndef NEWCUT_CENTRAL_WIDGET_H
#define NEWCUT_CENTRAL_WIDGET_H

#include <QFrame>
#include <QMdiArea>

namespace nc {
    class CentralWidget : public QFrame {
        Q_OBJECT

    public:
        CentralWidget(QWidget* parent);

        QMdiArea* GetMdiArea();

    protected:
        QMdiArea* mdi_area_;
    };
}


#endif //NEWCUT_CENTRAL_WIDGET_H
