//
// Created by wr on 2022/12/2.
//

#ifndef NEWCUT_DIAGRAM_TEXT_ITEM_H
#define NEWCUT_DIAGRAM_TEXT_ITEM_H

#include <QGraphicsTextItem>

namespace nc {
    class DiagramTextItem : public QGraphicsTextItem {
    Q_OBJECT
    public:
        enum { Type = UserType};

        DiagramTextItem(QGraphicsItem* parent = nullptr);

        int type() const override {
            return Type;
        }

    signals:
        void lostFocus(DiagramTextItem* item);

        void selectedChange(QGraphicsItem* item);

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

        void focusOutEvent(QFocusEvent* event) override;

        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    };
}

#endif //NEWCUT_DIAGRAM_TEXT_ITEM_H
