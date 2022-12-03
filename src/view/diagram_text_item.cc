//
// Created by wr on 2022/12/2.
//

#include "src/view/diagram_text_item.h"

namespace nc {
    DiagramTextItem::DiagramTextItem(QGraphicsItem* parent)
            : QGraphicsTextItem(parent) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
    }

    QVariant DiagramTextItem::itemChange(QGraphicsItem::GraphicsItemChange change,
                                         const QVariant& value) {
        if (change == QGraphicsItem::ItemSelectedHasChanged)
            emit selectedChange(this);

        return value;
    }

    void DiagramTextItem::focusOutEvent(QFocusEvent* event) {
        setTextInteractionFlags(Qt::NoTextInteraction);
        emit lostFocus(this);
    }

    void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
        if (textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
}
