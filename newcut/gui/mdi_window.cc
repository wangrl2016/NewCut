//
// Created by wr on 2023/1/11.
//

#include "newcut/engine/document.h"
#include "newcut/gui/mdi_window.h"

namespace nc {
    MdiWindow::MdiWindow(Document* doc, QWidget* parent, Qt::WindowFlags flags)
            : QMdiSubWindow(parent, flags) {
        setAttribute(Qt::WA_DeleteOnClose);
    }

    Document* MdiWindow::GetDocument() const {
        return document_;
    }
}