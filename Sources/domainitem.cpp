#include "Headers/domain.h"
#include "Headers/domainitem.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <QToolTip>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

//the only independent variables are the line's initial point and the angle

DomainItem::DomainItem(int id, QGraphicsItem* parent) : QGraphicsItem(parent) {

    this->id = id;
    int r, g, b;
    std::tie(r, g, b) = DOMAINS[id]->getRgbColor();

    randomlyInitialize();
    updatePath();

    for (int i = 0; i < 3; i++) this->grad[i] = 0.0;

    this->pen = QPen(QColor(r, g, b), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    this->brush = QBrush(Qt::SolidPattern);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->setAcceptHoverEvents(true);
}

DomainItem::~DomainItem() {}

void DomainItem::updatePath() {
    this->path = QPainterPath(this->line.p1());
    this->path.lineTo(this->line.p2());
    if (DOMAINS[this->id]->getNextId() == -1) {
        //this is the last domain in the strand, hence add an arrowhead
        QLineF tmp;
        tmp.setP1(QPointF(this->line.p2()));
        tmp.setAngle(this->line.angle()+135);
        tmp.setLength(5);
        this->path.lineTo(tmp.p2());
    }
}

QRectF DomainItem::boundingRect() const {
    return this->path.boundingRect();
}

void DomainItem::setGrad(double delx, double dely, double deltheta) {
    grad[0] = delx;
    grad[1] = dely;
    grad[2] = deltheta;
}

void DomainItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(this->pen);
    painter->drawPath(this->path);
}

void DomainItem::randomlyInitialize() {
    this->line = QLineF();
    this->line.setP1(QPointF(rand() % 50, rand() % 50));
    this->line.setAngle(rand() % 360);
    this->line.setLength(LENGTH);
    updatePath();
}

void DomainItem::updateLine(double alpha) {
    double x1 = this->line.x1() - alpha * grad[0],
            y1 = this->line.y1() - alpha * grad[1],
            theta = this->line.angle() - alpha * grad[2];
    this->line = QLineF();
    this->line.setP1(QPointF(x1, y1));
    this->line.setAngle(theta);
    this->line.setLength(LENGTH);
    updatePath();
    //now repaint the screen
    //std::cout << "repainting this domain" << std::endl;
    //update(this->boundingRect());
    //paint
    //usleep(50000000);
}

void DomainItem::setLine(double x, double y, double theta) {
    this->line = QLineF();
    this->line.setP1(QPointF(x, y));
    this->line.setAngle(theta);
    this->line.setLength(LENGTH);
    updatePath();
}

void DomainItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    std::ostringstream oss;
    oss << event->scenePos().x();
    oss << ", ";
    oss << event->scenePos().y();
    QToolTip::showText(QPoint(event->screenPos()), QString::fromStdString(DOMAINS[this->id]->getInfo() + oss.str()));
    //QToolTip::showText(QPoint(event->screenPos()), QString::fromStdString(DOMAINS[domain_id]->getInfo()));
}

void DomainItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QToolTip::hideText();
}

void DomainItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
}

void DomainItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

//in the functions where the line property is modified, callbacks should be added to change the path item as well
