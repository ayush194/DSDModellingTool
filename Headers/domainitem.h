#ifndef DOMAINITEM_H
#define DOMAINITEM_H

#include <QGraphicsItem>
#include <QPainter>

#define LENGTH 40

class DomainItem : public QGraphicsItem
{
    //Q_OBJECT
public:
    DomainItem(int, QGraphicsItem* = nullptr);
    ~DomainItem() override;
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void setGrad(double, double, double);
    void randomlyInitialize();
    void updateLine(double);
    void setLine(double, double, double);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QLineF line;

//signals:
//private slots:
private:
    int id;
    double grad[3];
    QPainterPath path;
    QPen pen;
    QBrush brush;

    void updatePath();
    //QPointF start_pos;
    //qreal theta;
    //uint length;
};

#endif // DOMAINITEM_H
