//
// Created by wangrl2016 on 2022/12/2.
//

#ifndef NEWCUT_DIAGRAM_ITEM_H
#define NEWCUT_DIAGRAM_ITEM_H

#include <QGraphicsPolygonItem>
#include <QList>
#include "src/view/arrow.h"

namespace nc {
    class Arrow;

    class DiagramItem : public QGraphicsPolygonItem {
    public:
        enum {
            Type = UserType + 15
        };

        enum DiagramType {
            Step,
            Conditional,
            StartEnd,
            Io
        };

        DiagramItem(DiagramType diagramType,
                    QMenu* contextMenu,
                    QGraphicsItem* parent = nullptr);

        void removeArrow(Arrow* arrow);

        void removeArrows();

        DiagramType diagramType() const {
            return myDiagramType;
        }

        QPolygonF polygon() const {
            return myPolygon;
        }

        void addArrow(Arrow* arrow);

        QPixmap image() const;

        int type() const override {
            return Type;
        }

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    private:
        DiagramType myDiagramType;
        QPolygonF myPolygon;
        QMenu* myContextMenu;
        QList<Arrow *> arrows;
    };
}

#endif //NEWCUT_DIAGRAM_ITEM_H
