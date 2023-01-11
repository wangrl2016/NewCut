//
// Created by wangrl2016 on 2023/1/11.
//

#include <QVBoxLayout>
#include "newcut/gui/central_widget.h"

namespace nc {
    CentralWidget::CentralWidget(QWidget* parent)
            : QFrame(parent), mdi_area_(new QMdiArea) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

        auto layout = new QVBoxLayout;
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(mdi_area_);

        mdi_area_->setObjectName("MdiArea");
        mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdi_area_->setFocusPolicy(Qt::ClickFocus);
        mdi_area_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mdi_area_->setActivationOrder(QMdiArea::ActivationHistoryOrder);
        mdi_area_->setTabsMovable(true);
        mdi_area_->setTabsClosable(true);

        setLayout(layout);
    }

    QMdiArea* CentralWidget::GetMdiArea() {
        return mdi_area_;
    }
}
