//
// Created by wr on 2023/1/11.
//

#ifndef NEWCUT_MDI_WINDOW_H
#define NEWCUT_MDI_WINDOW_H

#include <QMdiSubWindow>

namespace nc {
    class Document;

    // Multiple Document Interface
    class MdiWindow : public QMdiSubWindow {
    Q_OBJECT
    public:
        MdiWindow(Document* doc,
                  QWidget* parent,
                  Qt::WindowFlags flags = Qt::WindowType::Widget);

        Document* GetDocument() const;

    private:
        Document* document_;
    };
}

#endif //NEWCUT_MDI_WINDOW_H
