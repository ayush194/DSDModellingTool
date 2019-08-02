#include "Headers/dashboardscene.h"

#define X_MIN -1000
#define X_MAX 1000
#define Y_MIN -1000
#define Y_MAX 1000

DashboardScene::DashboardScene(QObject* parent) : QGraphicsScene(parent) {
    //create lines with spacing of 50 in between them
    //create dashed lines with spacing of 10 in between them
    //put 10 lines parallel to x-axis and 10 lines parallel to y-axis
    for(int x = X_MIN; x <= X_MAX; x += 50) {
        Qt::PenStyle style = Qt::DashLine;
        double width = 1;
        if (x % 100 == 0) {style = Qt::SolidLine; width = 1.5;}
        this->addLine(QLine(x, Y_MIN, x, Y_MAX), QPen(QColor(0, 0, 0), width, style, Qt::RoundCap, Qt::RoundJoin));
    }
    for(int y = Y_MIN; y <= Y_MAX; y += 50) {
        Qt::PenStyle style = Qt::DashLine;
        double width = 1;
        if (y % 100 == 0) {style = Qt::SolidLine; width = 1.5;}
        this->addLine(QLine(X_MIN, y, X_MAX, y), QPen(QColor(0, 0, 0), width, style, Qt::RoundCap, Qt::RoundJoin));
    }
    this->addEllipse(QRectF(QPoint(-1, 1), QPoint(1, -1)), QPen(), QBrush());
    setSceneRect(QRect(QPoint(-100, 100), QPoint(100, -100)));
}
