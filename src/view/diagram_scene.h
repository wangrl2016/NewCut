//
// Created by wangrl2016 on 2022/12/1.
//

#ifndef NEWCUT_DIAGRAM_SCENE_H
#define NEWCUT_DIAGRAM_SCENE_H

#include <QGraphicsScene>
#include <QMenu>

#include "src/view/diagram_text_item.h"
#include "src/view/diagram_item.h"

namespace nc {
    class DiagramScene : public QGraphicsScene {
    Q_OBJECT
    public:
        enum Mode {
            InsertItem,
            InsertLine,
            InsertText,
            MoveItem
        };

        explicit DiagramScene(QMenu* itemMenu, QObject* parent = nullptr);

        QFont font() const { return myFont; }

        QColor textColor() const { return myTextColor; }

        QColor itemColor() const { return myItemColor; }

        QColor lineColor() const { return myLineColor; }

        void setLineColor(const QColor& color);

        void setTextColor(const QColor& color);

        void setItemColor(const QColor& color);

        void setFont(const QFont& font);

    public slots:

        void setMode(Mode mode);

        void setItemType(DiagramItem::DiagramType type);

        void editorLostFocus(DiagramTextItem* item);

    signals:

        void itemInserted(DiagramItem* item);

        void textInserted(QGraphicsTextItem* item);

        void itemSelected(QGraphicsItem* item);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

        void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

        void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    private:
        bool isItemChange(int type) const;

    private:
        DiagramItem::DiagramType myItemType;
        QMenu* myItemMenu;
        Mode myMode;
        bool leftButtonDown;
        QPointF startPoint;
        QGraphicsLineItem* line;
        QFont myFont;
        DiagramTextItem* textItem;
        QColor myTextColor;
        QColor myItemColor;
        QColor myLineColor;
    };
}

#endif //NEWCUT_DIAGRAM_SCENE_H
