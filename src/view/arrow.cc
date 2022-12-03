//
// Created by wr on 2022/12/2.
//

#include <QPainter>
#include <QPen>
#include <QtMath>
#include "src/view/arrow.h"
#include "src/view/diagram_item.h"

namespace nc {
    Arrow::Arrow(nc::DiagramItem* startItem,
                 nc::DiagramItem* endItem,
                 QGraphicsItem* parent)
            : QGraphicsLineItem(parent), myStartItem(startItem),
              myEndItem(endItem) {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    QRectF Arrow::boundingRect() const {
        qreal extra = (pen().width() + 20) / 2.0;

        return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                          line().p2().y() - line().p1().y()))
                .normalized()
                .adjusted(-extra, -extra, extra, extra);
    }

    QPainterPath Arrow::shape() const {
        QPainterPath path = QGraphicsLineItem::shape();
        path.addPolygon(arrowHead);
        return path;
    }

    void Arrow::updatePosition() {
        QLineF line(mapFromItem(myStartItem, 0.0, 0.0),
                    mapFromItem(myEndItem, 0, 0));
        setLine(line);
    }

    void Arrow::paint(QPainter* painter,
                      const QStyleOptionGraphicsItem* option,
                      QWidget* widget) {
        if (myStartItem->collidesWithItem(myEndItem))
            return;

        QPen myPen = pen();
        myPen.setColor(myColor);
        qreal arrowSize = 20;
        painter->setPen(myPen);
        painter->setBrush(myColor);

        QLineF centerLine(myStartItem->pos(), myEndItem->pos());
        QPolygonF endPolygon = myEndItem->polygon();
        QPointF p1 = endPolygon.first() + myEndItem->pos();
        QPointF intersectPoint;
        for (int i = 1; i < endPolygon.count(); i++) {
            QPointF p2 = endPolygon.at(i) + myEndItem->pos();
            QLineF polyLine = QLineF(p1, p2);
            QLineF::IntersectionType intersectionType =
                    polyLine.intersects(centerLine, &intersectPoint);
            if (intersectionType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }

        setLine(QLineF(intersectPoint, myStartItem->pos()));

        double angle = std::atan2(-line().dy(), line().dx());

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                cos(angle + M_PI - M_PI / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected()) {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0, -8.0);
            painter->drawLine(myLine);
        }
    }
}