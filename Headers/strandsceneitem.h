#ifndef STRANDSCENEITEM_H
#define STRANDSCENEITEM_H

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>

#define DISTANCE 10
#define LENGTH 40
#define TOEHOLD_RENDER_LENGTH 30
#define RECOGNITION_RENDER_LENGTH 40

class StrandSceneItem : public QGraphicsItem {
public:
    StrandSceneItem(int domain_id, const QPainterPath&, const QPen& = QPen(), const QBrush& = QBrush(), QGraphicsItem* = nullptr);
    StrandSceneItem(int domain_id, const QPainterPath&, const QColor&, QGraphicsItem* = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void resetPenColor();
    int getDomainId();
    QPainterPath& getPath();

    //reimplement the slots for hover and click
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int domain_id;
    QPainterPath path;
    QPen pen;
    QBrush brush;
};

#endif // STRANDSCENEITEM_H
