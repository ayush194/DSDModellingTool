#include "Headers/strandsceneitem.h"
#include <iostream>
#include <QPainter>
#include <QToolTip>
#include <QGraphicsSceneHoverEvent>
#include "Headers/domain.h"
#include <string>
#include <sstream>

StrandSceneItem::StrandSceneItem(int id, const QPainterPath &path, const QPen &pen, const QBrush &brush, QGraphicsItem *parent) : QGraphicsItem(parent) {
    this->path = path;
    this->pen = pen;
    this->brush = brush;
    this->domain_id = id;
    //setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->setAcceptHoverEvents(true);
}

StrandSceneItem::StrandSceneItem(int id, const QPainterPath &path, const QColor &color, QGraphicsItem *parent) : QGraphicsItem (parent) {
    this->path = path;
    this->pen = QPen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    this->brush = QBrush(Qt::SolidPattern);
    this->domain_id = id;
    //this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->setAcceptHoverEvents(true);
}

void StrandSceneItem::resetPenColor() {
    int r, g, b;
    std::tie(r, g, b) = DOMAINS[domain_id]->getRgbColor();
    this->pen.setColor(QColor(r, g, b));
}

//reimplementing virtual methods
QRectF StrandSceneItem::boundingRect() const {
    return this->path.boundingRect();
}

void StrandSceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(this->pen);
    //painter->setBrush(this->brush);
    painter->drawPath(this->path);
    //painter->drawRect(this->boundingRect());
}

int StrandSceneItem::getDomainId() {
    return this->domain_id;
}

QPainterPath& StrandSceneItem::getPath() {
    return this->path;
}

void StrandSceneItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    std::cout << "hoverenterevent" << std::endl;
    std::ostringstream oss;
    oss << event->scenePos().x();
    oss << ", ";
    oss << event->scenePos().y();
    QToolTip::showText(QPoint(event->screenPos()), QString::fromStdString(DOMAINS[domain_id]->getInfo() + oss.str()));
    if (this->isSelected()) return;
    //set pen color of the item to black
    if (this->domain_id < 0) {
        //
    }
    this->pen.setColor(QColor(255, 255, 255));
    update();
    //QToolTip::showText(QPoint(event->screenPos()), QString::fromStdString(DOMAINS[domain_id]->getInfo()));
}

void StrandSceneItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    std::cout << "hoverleaveevent" << std::endl;
    if (this->isSelected()) return;
    //set pen color of the item back to original
    this->resetPenColor();
    update();
    QToolTip::hideText();
}

void StrandSceneItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << "mousepressevent" << std::endl;
}

void StrandSceneItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << "mousereleaseevent" << std::endl;
    if (this->isSelected()) {
        //if this item is already selected, then deselect it and reset its pen color
        this->setSelected(false);
        this->resetPenColor();
    } else {
        //if this item is not already selected, then select it and set its pen color as black
        this->setSelected(true);
        this->pen.setColor(QColor(0, 0, 0));
    }
    update();
}



