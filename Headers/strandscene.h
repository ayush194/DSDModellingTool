//
//  strandscene.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 4/6/19.
//

#ifndef STRANDSCENE_H
#define STRANDSCENE_H

#include <set>
#include <unordered_set>
#include <QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include "strandsceneitem.h"

class StrandScene : public QGraphicsScene {
    //this is the place where everything will be rendered
    //a strandscene will consist of strandscenepathitems
    Q_OBJECT
public:
    explicit StrandScene(QObject *parent = nullptr);
    ~StrandScene();
    QRectF combineRects(const QRectF&, const QRectF&) const;
    QRectF boundingRect(const std::vector<StrandSceneItem*>&) const;
    void translate(const std::vector<StrandSceneItem*>&, qreal, qreal);
    void clearScene();
    void resetPenPos();
    StrandSceneItem* createDomainItem(int, const QPointF&, qreal);
    StrandSceneItem* createDotItem(int, int, const QPointF&, const QColor&);
    //QList<StrandSceneItem*> createHairpinLoopItems(std::vector<int>, QPointF&, qreal);
    //QList<StrandSceneItem*> createBulgeLoopItems(std::vector<int>, QPointF&, qreal);
    std::vector<StrandSceneItem*> createHairpinLoop(int, QPointF, QPointF);
    std::vector<StrandSceneItem*> createBulgeLoop(int, QPointF, qreal);
    void renderStrand(int);
    void renderSpecies(int);
    //QPointF calculateComplementaryPos(QPointF, qreal);
    QPointF calculateNewPos(const QPointF&, qreal, uint);

//signals:
//private slots:

public slots:
    void createStrand(int id);
    void createSpecies(int id);
    void render(const QString &text);

private:
    //int curr_dirn;
    QPointF prev_pos;
    //prev_pos is the position where the rendering of the 2nd last domain ended
    QPointF curr_pos;
    //curr_pos is the position where the rendering of the last domain ended
    //note that prev_pos or curr_pos doesn't include the position of the small semi-arrow
    //in cast the last/2nd last rendered domain was at the 3' end
    QPen *pen;
    QBrush *brush;
    //each domain maps to a unique StrandSceneItem
    //each domain_id can have only one strandsceneitem associatd with it, because if let's say
    //there are two strandsceneitems associated with a single domain, then the user could add
    //both of them to the scene, and join them, which would make no sense. To keep this in coherence
    //with user-friendliness and rationality, there should be only one instance of StrandSceneItem for
    //each domain
    std::map<int, StrandSceneItem*> strandsceneitems;
    //Shape *activeshape;
    //activeshape should contain all info required to draw the currently selected species...
    //on instanciation of the StrandScene class, it should draw the activeshape it holds
};

#endif // StrandScene_H
