//
//  strandscene.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 4/6/19.
//

#include <unistd.h>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <sstream>
#include <iostream>
#include "Headers/strandscene.h"
#include "Headers/strandsceneitem.h"
#include "Headers/strand.h"
#include "Headers/species.h"
#include "Headers/domain.h"
#include <QPainter>
#include <iostream>

//The scene is an inifinite 2d plane with an origin, an x-axis and a y-axis, and the plane extending to infinity.
//The coordinates you specify when drawing the path using Qpainter are the coordinates on this plane
//when the GraphicsView presents the scene, if finds out the boundingRect() for the scene by taking into account
//the boundingRects of all the items on the scene, then it places the boundingRect so that it's in the center of the screen

StrandScene::StrandScene(QObject *parent) : QGraphicsScene(parent) {
    pen = new QPen(QColor(0, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    brush = new QBrush(Qt::SolidPattern);
    clearScene();
    resetPenPos();
    //create all the existing items
    for(auto it = dsd::Strand::getObjects().begin(); it != dsd::Strand::getObjects().end(); it++) {
        if (it->second->getSpeciesId() == -1) {
            //isolated strand
            createStrand(it->first);
        }
    }
    for(auto it = dsd::Species::getObjects().begin(); it != dsd::Species::getObjects().end(); it++) {
        createSpecies(it->first);
    }
    //this->addLine(40, 40, 40, 40, *pen);
    //setSceneRect(400, 300);
    //setAutoFillBackground(true);
    //QPalette pal = palette();
    //pal.setColor(QPalette::Background, Qt::white);
    //setPalette(pal);
    //update();
}

StrandScene::~StrandScene() {
    pen->~QPen();
    brush->~QBrush();
}

/*
void StrandScene::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    //QPen pen(Qt::SolidLine);
    //QBrush brush(Qt::Dense1Pattern);
    //painter.setBrush(brush);
    painter.setPen(QPen(QColor(255, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(0, 0, 200, 200);
}
*/

StrandSceneItem* StrandScene::createDomainItem(int id, const QPointF &start_pos, qreal dirn) {
    //renders the given domain, at the position specified by start_pos and in the direction given by dirn.
    //depending on whether it is the last domain in the strand, it will be rendered with a semi arrow
    //domains are always rendered in the direction of the strand i.e along the direction of the arrow
    dsd::Domain *d = dsd::Domain::getObjects()[id];
    //length of the domains should depend on the actual sequence length
    //but right now toehold domains have a length of 30 while normal recognition domains are 40
    int l;
    //if (d->isToehold()) l = TOEHOLD_RENDER_LENGTH; else l = RECOGNITION_RENDER_LENGTH;
    l = LENGTH;
    bool is_last_domain = id == dsd::Strand::getObjects()[dsd::Domain::getObjects()[id]->getStrandId()]->getDomains().back();
    QLineF angleline;
    angleline.setP1(start_pos);
    angleline.setAngle(dirn);
    angleline.setLength(l);
    prev_pos = start_pos;
    curr_pos = angleline.p2();
    int r, g, b;
    std::tie(r, g, b) = d->getRgbColor();
    //pen->setColor(QColor(r, g, b));
    QPainterPath path(prev_pos);
    if (is_last_domain) {
        //draw the domain
        path.lineTo(curr_pos);
        QLineF tmpline;
        tmpline.setP1(curr_pos);
        tmpline.setAngle(dirn + 135);
        tmpline.setLength(10);
        //draw the semi arrow
        path.lineTo(tmpline.p2());
        //this->addPath(path, *pen);
    } else {
        path.lineTo(angleline.p2());
    }
    StrandSceneItem *newitem = new StrandSceneItem(id, path, QColor(r, g, b));
    strandsceneitems[id] = newitem;
    //add newitem to the list of items (but without rendering it)
    //strandsceneitems[id] = newitem;
    return newitem;
}

/*
QList<StrandSceneItem*> StrandScene::createHairpinLoopItems(std::vector<int> domain_ids, QPointF& start_point, qreal dirn) {

}

QList<StrandSceneItem*> StrandScene::createBulgeLoopItems(std::vector<int> domains_ids, QPointF& start_point, qreal dirn) {

}
*/

std::vector<StrandSceneItem*> StrandScene::createHairpinLoop(int domain_id, QPointF start_pos, QPointF end_pos) {
    //the direction of vector start_pos -> end_pos is in the direction of th strand
    //create a hairpinloop starting from domain_id (this has to be the start of the loop)
    //the starting and ending of the loop should be start_pos and end_pos
    //domain_id will come at the start_pos
    //need to find all consecutive domains which are part of the hairpin loop
    //get the center of the circle
    int curr_domain_id = domain_id;
    std::vector<int> domains_in_hairpin;
    std::vector<StrandSceneItem*> hairpin_items;
    while(DOMAINS[curr_domain_id]->getShape() == dsd::hairpin) {
        domains_in_hairpin.push_back(curr_domain_id);
        curr_domain_id = DOMAINS[curr_domain_id]->getNextId();
    }
    QLineF tmpvec1(start_pos, end_pos);
    double len = tmpvec1.length();
    QLineF tmpvec2;
    tmpvec2.setP1(start_pos);
    tmpvec2.setAngle(tmpvec1.angle() + 60);
    tmpvec2.setLength(len);
    QPointF center(tmpvec2.p2());
    //create a square with center as centre and 2 * tmpvec1.length() as side length
    //and then create a circle inside the square
    //divide the circumference i.e. 300 in equal parts and then one by one draw the clockwise arcs
    QRectF bounding_square(center.x() - len, center.y() - len, 2*len, 2*len);
    qreal start_angle = tmpvec1.angle()+60+180;
    qreal span_angle = 300.0 / domains_in_hairpin.size();
    QPainterPath mypath = QPainterPath(start_pos);
    for(int dom_id : domains_in_hairpin) {
        //currently mypath is at the starting position of this domain
        //sweep an angle of span_angle by moving clockwise
        mypath.arcTo(bounding_square, start_angle, -span_angle);
        int r, g, b;
        std::tie(r, g, b) = DOMAINS[dom_id]->getRgbColor();
        StrandSceneItem *newitem = new StrandSceneItem(dom_id, mypath, QColor(r, g, b));
        hairpin_items.push_back(newitem);
        start_angle -= span_angle;
        mypath = QPainterPath(mypath.pointAtPercent(1));
    }
    return hairpin_items;
}

std::vector<StrandSceneItem*> StrandScene::createBulgeLoop(int domain_id, QPointF start_pos, qreal dirn) {
    //create a bulge loop starting from domain_id (this has to be the start of the loop)
    //the bulge will start at start_pos and end at start_pos
    std::cout << "Starting position is " << start_pos.x() << ' ' << start_pos.y() << ' ' << dirn << std::endl;
    std::vector<int> domains_in_bulge;
    std::vector<StrandSceneItem*> bulge_items;
    int curr_domain_id = domain_id;
    while(DOMAINS[curr_domain_id]->getShape() == dsd::bulge) {
        domains_in_bulge.push_back(curr_domain_id);
        curr_domain_id = DOMAINS[curr_domain_id]->getNextId();
    }
    QLineF tmpvec;
    double radius = 10;
    tmpvec.setP1(start_pos);
    tmpvec.setAngle(dirn+90);
    tmpvec.setLength(radius);
    QPointF center(tmpvec.p2());
    QRectF bounding_square(center.x() - radius, center.y() - radius, 2*radius, 2*radius);
    qreal start_angle = tmpvec.angle()+180;
    qreal span_length = 1.0 / domains_in_bulge.size() * 2 * M_PI * radius;
    qreal span_angle = 360.0 / domains_in_bulge.size();
    std::cout << "span angle " << span_angle << std::endl;
    QPainterPath mypath(start_pos);
    for(int dom_id : domains_in_bulge) {
        //std::cout << "domain in bulge " << dom_id << std::endl;
        mypath.arcTo(bounding_square, start_angle, -span_angle);
        int r, g, b;
        std::tie(r, g, b) = DOMAINS[dom_id]->getRgbColor();
        StrandSceneItem *newitem = new StrandSceneItem(dom_id, mypath, QColor(r,g,b));
        bulge_items.push_back(newitem);
        start_angle -= span_angle;
        mypath = QPainterPath(mypath.pointAtPercent(1));
    }
    for(auto item : bulge_items) {
        std::cout << "just one more check " << item->getDomainId() << std::endl;
    }
    return bulge_items;
}

//to render a curved domain, we need the boundingRect, startangle and length

StrandSceneItem* StrandScene::createDotItem(int id1, int id2, const QPointF &pos, const QColor &color) {
    //draws a dot at the given position and of the given color
    //each dot is associated with two domains, but how do we store that
    //pen->setColor(color);
    //brush->setColor(color);
    QPainterPath dot;
    dot.addEllipse(pos.x()-1, pos.y()-1, 2, 2);
    StrandSceneItem *newitem = new StrandSceneItem(id1, dot, color);
    strandsceneitems[-id1] = newitem;
    strandsceneitems[-id2] = newitem;
    return newitem;
    //this->addEllipse(pos.x()-1, pos.y()-1, 2, 2, *pen, *brush);
}

/*
QPointF calculateComplementaryPos(QPointF &end_pos, qreal dirn) {
    //given the end_pos of a domain, it calculates the start_pos of the complementary domain
    QLineF line;
    line.setP1(end_pos);
    line.setAngle(dirn-90);
    line.setLength(10);
    //currently, the separation between the two strands is set to 10
    return line.p2();
}
*/

QPointF StrandScene::calculateNewPos(const QPointF &pos, qreal dirn, uint length) {
    QLineF line;
    line.setP1(pos);
    line.setAngle(dirn);
    line.setLength(length);
    return line.p2();
}

void StrandScene::createSpecies(int id) {
    //resetScene();
    //render all the strands of the given species
    //algorithm for rendering a species
    //enumerate all the strands, let there be n strands
    //set rotangle = 360/(n+1)
    //render each strand one by one
    //for each strand, loop over the domains and depending whether the previous bound_domain belongs to
    //the same strand or not, use the rotangle to change the directions
    //when one strand is done, pick up the last rendered domain which was bound
    //from there start rendering that strand

    //queue<tuple<domain_id, start_pos, dirn>
    //render domains one by one, if the currenty rendered domain is bound to another
    //then push,

    //just render it the way you render a normal graph using bfs/dfs
    //this graph has two types of edges
    //one is the normal covalent bond edge and the other is the h-bond edge
    //modifications to the normal bfs approach
    //along with the starting position, dirn, Strand*, idx_of_domain


    std::unordered_set<int> strands;
    for(int strand_id : SPECIES[id]->getStrands()) {
        strands.insert(strand_id);
    }

    //int first_strand_id = SPECIES[id]->getStrands()[0];
    //dsd::Strand* first_strand = STRANDS[first_strand_id];
    //std::queue<std::tuple<dsd::Strand*, int, QPointF, qreal>> renderer;
    std::queue<std::tuple<int, QPointF, qreal>> renderer;
    std::unordered_set<int> rendered_domains;
    std::vector<StrandSceneItem*> items;
    QRectF prev_br, curr_br;
    bool flag = true;
    while (!strands.empty()) {
        clearScene();
        resetPenPos();
        renderer.push(std::make_tuple(STRANDS[*strands.begin()]->getDomains()[0], curr_pos, 0));
        //rendered_domains.clear();
        items.clear();
        //find the number of strands in this connected component
        uint n = dsd::Strand::numberOfStrandsInConnectedComponent(std::get<0>(renderer.front()));
        qreal rotation_angle = 180 - 360/n;
        //qreal rotation_angle = 90;
        while(!renderer.empty()) {
            //this while loop traverses one connected component
            auto top = renderer.front();
            renderer.pop();
            //dsd::Strand* curr_strand;
            int domain_id;
            std::tie(domain_id, std::ignore, std::ignore) = top;
            strands.erase(DOMAINS[domain_id]->getStrandId());
            //int domain_id = curr_strand->getDomains()[domain_idx];
            if (rendered_domains.find(domain_id) != rendered_domains.end()) {
                //this domain has already been rendered
                continue;
            }
            //now this domain will be rendered;
            QPointF start_pos = std::get<1>(top);
            qreal dirn = std::get<2>(top);
            /*
            dsd::domainshape sh = DOMAINS[domain_id]->getShape();
            int curr_domain_id = domain_id;
            std::vector<int> domains_with_same_shape;
            while(curr_domain_id != -1 && DOMAINS[curr_domain_id]->getShape() == sh) {
                domains_with_same_shape.push_back(curr_domain_id);
                curr_domain_id = DOMAINS[curr_domain_id]->getNextId();
            }
            */
            //if (sh == dsd::bulge) {createBulgeLoop(domain_id, start_pos, dirn);}
            //else if (sh == dsd::hairpin) {createHairpinLoop(domain_id, start_pos, dirn);}
            //if (sh == dsd::bulge) {createBulgeLoopItems(domains_with_same_shape, start_pos, dirn); continue;}
            //else if (sh == dsd::hairpin) {createHairpinLoopItems(domains_with_same_shape, start_pos, dirn); continue;}
            std::cout << "rendering domain " << domain_id << " location x " << start_pos.x() << " y " << start_pos.y() << std::endl;
            items.push_back(createDomainItem(domain_id, start_pos, dirn));
            rendered_domains.insert(domain_id);
            //now push the unrendered neighbors of this domain and in case it has a bound domain
            //then render the dot for the bound pair of domains
            int bound_domain_id = DOMAINS[domain_id]->getBoundId();
            int bound_domain_strand_id = -1;
            if (bound_domain_id != -1) {
                //there exists a domain bound with this
                bound_domain_strand_id = DOMAINS[bound_domain_id]->getStrandId();
                if (rendered_domains.find(bound_domain_id) != rendered_domains.end()) {
                    //the complementary domain has already been rendered
                    //no need to render the dot indicating the bond
                } else {
                    //the complementary domain has not been rendered
                    QPointF bound_domain_start_pos = calculateNewPos(curr_pos, dirn-90, DISTANCE);
                    std::cout << "pushing domain " << bound_domain_id << " location x " << bound_domain_start_pos.x() << " y " << bound_domain_start_pos.y() << std::endl;
                    renderer.push(std::make_tuple(bound_domain_id, bound_domain_start_pos, dirn+180));
                    //now render the dot between the complementary domains
                    QPointF dot_pos = calculateNewPos(QPointF((curr_pos.x()+prev_pos.x())/2, (curr_pos.y()+prev_pos.y())/2), dirn-90, 5);
                    int r, g, b;
                    std::tie(r, g, b) = DOMAINS[domain_id]->getRgbColor();
                    QColor dot_color(r, g, b);
                    items.push_back(createDotItem(domain_id, bound_domain_id, dot_pos, dot_color));
                }
            }
            //push the prev and next domains
            //if the prev_domain is a hairpin or a bule, then skip it
            //if the nex_domain is a hairpin or a bulge, then create it without pushing it
            //{unbound : unbound, bound}, {bound : unbound, bound_to_same, bound_to_different}
            int new_domain_id[2] = {DOMAINS[domain_id]->getPrevId(), DOMAINS[domain_id]->getNextId()};
            qreal new_domain_dirn[2][2][3] = {{{dirn, dirn - rotation_angle/2, 0}, {dirn - rotation_angle/2, dirn, dirn - rotation_angle}},
                                             {{dirn, dirn + rotation_angle/2, 0}, {dirn + rotation_angle/2, dirn, dirn + rotation_angle}}};
            for (uint i : {0, 1}) {
                if (new_domain_id[i] == -1) continue;
                if (i == 0 && DOMAINS[new_domain_id[0]]->getShape() == dsd::hairpin) continue;
                if (i == 0 && DOMAINS[new_domain_id[0]]->getShape() == dsd::bulge) continue;
                if (i == 1) {
                    std::vector<StrandSceneItem*> newitems;
                    bool loop_found = true;
                    dsd::domainshape sh = DOMAINS[new_domain_id[1]]->getShape();
                    if (sh == dsd::hairpin) {
                        newitems = createHairpinLoop(new_domain_id[1], calculateNewPos(start_pos, dirn, LENGTH), calculateNewPos(calculateNewPos(start_pos, dirn, LENGTH), dirn-90, DISTANCE));
                    } else if (sh == dsd::bulge) {
                        newitems = createBulgeLoop(new_domain_id[1], calculateNewPos(start_pos, dirn, LENGTH), dirn);
                    } else {
                        loop_found = false;
                    }
                    if (loop_found) {
                        int last_domain_id = -1;
                        for(auto item : newitems) {
                            std::cout << "bla bla " << item->getDomainId() << std::endl;
                            strandsceneitems[item->getDomainId()] = item;
                            items.push_back(item);
                            rendered_domains.insert(item->getDomainId());
                            last_domain_id = item->getDomainId();
                        }
                        //push the next neighbor of last_domain_id
                        if (sh == dsd::hairpin) {
                            renderer.push(std::make_tuple(DOMAINS[last_domain_id]->getNextId(), calculateNewPos(calculateNewPos(start_pos, dirn, LENGTH), dirn-90, DISTANCE), dirn+180));
                        } else if (sh == dsd::bulge) {
                            renderer.push(std::make_tuple(DOMAINS[last_domain_id]->getNextId(), calculateNewPos(start_pos, dirn, LENGTH), dirn));
                        }
                        continue;
                    }
                }
                if (rendered_domains.find(new_domain_id[i]) == rendered_domains.end()) {
                    //currently trying to push new_domain_id[i];
                    uint j, k;
                    enum bind_state {unbound = 0, bound};
                    if (DOMAINS[new_domain_id[i]]->getBoundId() == -1) {
                        j = unbound;
                        if (DOMAINS[domain_id]->getBoundId() == -1) k = 0;
                        else k = 1;
                    } else {
                        j = bound;
                        if (DOMAINS[domain_id]->getBoundId() == -1) k = 0;
                        else if (DOMAINS[DOMAINS[domain_id]->getBoundId()]->getStrandId() == DOMAINS[DOMAINS[new_domain_id[i]]->getBoundId()]->getStrandId()) k = 1;
                        else k = 2;
                    }
                    uint l;
                    qreal new_dirn = new_domain_dirn[i][j][k];
                    //if (DOMAINS[new_domain_id[i]]->isToehold()) l = TOEHOLD_RENDER_LENGTH; else l = RECOGNITION_RENDER_LENGTH;
                    l = LENGTH;
                    //std::cout << dirn << std::endl;
                    if (i == 0) renderer.push(std::make_tuple(new_domain_id[i], calculateNewPos(prev_pos, new_dirn+180, l), new_dirn));
                    else renderer.push(std::make_tuple(new_domain_id[i], curr_pos, new_dirn));
                    //std:: cout << "pushing domain " << prev_domain_id << " location x " << prev_domain_start_pos.x() << " y " << prev_domain_start_pos.y() << std::endl;
                }
            }
            /*
            if (DOMAINS[domain_id]->getPrevId() != -1) {
                //this domain is not the first domain of the strand, so push the domain before it
                //the prev_domain_dirn has to be decided depending on the bound_strand_ids of domain_idx and domain_idx-1
                qreal prev_domain_dirn;
                int prev_domain_id = DOMAINS[domain_id]->getPrevId();
                if (rendered_domains.find(prev_domain_id) == rendered_domains.end()) {
                    //this domain hasn't been rendered, so it will be pushed into the queue
                    int prev_domain_bound_domain_id = DOMAINS[prev_domain_id]->getBoundId();
                    int prev_domain_bound_domain_strand_id = -1;
                    if (prev_domain_bound_domain_id == -1)  {
                        //previous domain is free
                        if (bound_domain_strand_id == -1) {
                            //current domain is free, and so is previous domain
                            prev_domain_dirn = dirn;
                        } else {
                            //current domain is bound, but previous domain is free
                            prev_domain_dirn = dirn - rotation_angle/2;
                        }
                    } else {
                        //previous domain is bound
                        prev_domain_bound_domain_strand_id = DOMAINS[prev_domain_bound_domain_id]->getStrandId();
                        if (bound_domain_strand_id == -1) {
                            //current domain is free, and previous domain is bound
                            prev_domain_dirn = dirn - rotation_angle/2;
                        } else if (bound_domain_strand_id == prev_domain_bound_domain_strand_id){
                            //current domain is bound, and previous domain is bound to the same strand
                            prev_domain_dirn = dirn;
                        } else {
                            //current domain is bound, and previous domain is bound but to different strands
                            prev_domain_dirn = dirn - rotation_angle;
                        }
                    }
                    int l;
                    if (DOMAINS[prev_domain_id]->isToehold()) l = TOEHOLD_RENDER_LENGTH; else l = RECOGNITION_RENDER_LENGTH;
                    std::cout << prev_domain_dirn << std::endl;
                    QPointF prev_domain_start_pos = calculateNewPos(prev_pos, prev_domain_dirn+180, l);
                    std:: cout << "pushing domain " << prev_domain_id << " location x " << prev_domain_start_pos.x() << " y " << prev_domain_start_pos.y() << std::endl;
                    renderer.push(std::make_tuple(prev_domain_id, prev_domain_start_pos, prev_domain_dirn));
                }
            }
            if (DOMAINS[domain_id]->getNextId() != -1) {
                //this domain is not the last domain of the current strand, so push the domain after it
                //the next_domain_dirn has to be decided depending on the bound_strand_ids of domain_idx and domain_idx+1
                qreal next_domain_dirn;
                int next_domain_id = DOMAINS[domain_id]->getNextId();
                if (rendered_domains.find(next_domain_id) == rendered_domains.end()) {
                    //this domain hasn't been rendered
                    int next_domain_bound_domain_id = DOMAINS[next_domain_id]->getBoundId();
                    int next_domain_bound_domain_strand_id = -1;
                    if (next_domain_bound_domain_id == -1)  {
                        //next domain is free
                        if (bound_domain_strand_id == -1) {
                            //current domain is free, and so is next domain
                            next_domain_dirn = dirn;
                        } else {
                            //current domain is bound, but next domain is free
                            next_domain_dirn = dirn + rotation_angle/2;
                        }

                    } else {
                        //previous domain is bound
                        next_domain_bound_domain_strand_id = DOMAINS[next_domain_bound_domain_id]->getStrandId();
                        if (bound_domain_strand_id == -1) {
                            //current domain is free, and next domain is bound
                            next_domain_dirn = dirn + rotation_angle/2;
                        } else if (bound_domain_strand_id == next_domain_bound_domain_strand_id){
                            //current domain is bound, and next domain is bound to the same strand
                            next_domain_dirn = dirn;
                        } else {
                            //current domain is bound, and next domain is bound but to different strands
                            next_domain_dirn = dirn + rotation_angle;
                        }
                    }
                    std:: cout << "pushing domain " << next_domain_id << " location x " << curr_pos.x() << " y " << curr_pos.y() << std::endl;
                    //QPointF next_domain_bound_domain_start_pos = calculateNewPos(prev_pos, dirn+180, 30);
                    renderer.push(std::make_tuple(next_domain_id, curr_pos, next_domain_dirn));
                }
            }
            */
        }
        std::cout << "size of items " << items.size() << std::endl;
        for(auto item : items) {
            std::cout << "counting items" << std::endl;
            std::cout << item->getDomainId() << std::endl;
        }
        if (flag) {
            //this is the first connected component so prev_br is unset
            prev_br = boundingRect(items);
            std::cout << prev_br.topLeft().x() << ' ' << prev_br.topLeft().y() << std::endl;
            flag = false;
        } else {
            curr_br = boundingRect(items);
            std::cout << curr_br.topLeft().x() << ' ' << curr_br.topLeft().y() << std::endl;
            translate(items, prev_br.topLeft().x()-curr_br.topLeft().x(),
                      prev_br.bottomRight().y()-curr_br.topLeft().y()+10);
            curr_br = boundingRect(items);
            std::cout << curr_br.topLeft().x() << ' ' << curr_br.topLeft().y() << std::endl;
            //set prev_br as the combined br of curr and prev
            prev_br = combineRects(prev_br, curr_br);
        }
    }
    std::cout << "rendering ended" << std::endl;
}

void StrandScene::createStrand(int id) {
    clearScene();
    resetPenPos();
    //only create the strand without rendering it
    const std::vector<int>& domains = dsd::Strand::getObjects()[id]->getDomains();
    for(uint i = 0; i < domains.size(); i++) {
        //create all the domains in this strand
        createDomainItem(domains[i], curr_pos, 0);
        //renderDomain(domains[i], curr_pos, 0);
    }
}

void StrandScene::renderStrand(int domain_id) {
    clearScene();
    //std::vector<StrandSceneItem*> items;
    for(int domain_id : dsd::Strand::getObjects()[domain_id]->getDomains()) {
        this->addItem(this->strandsceneitems[domain_id]);
        //items.push_back(this->strandsceneitems[domain_id]);
        //this->addRect(this->strandsceneitems[domain_id]->boundingRect());
    }
    //this->addRect(boundingRect(items));
    //this->addRect(this->itemsBoundingRect());
    setSceneRect(itemsBoundingRect());
}

void StrandScene::renderSpecies(int species_id) {
    clearScene();
    std::unordered_set<int> rendered_domains;
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        //render all domains in this strand;
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            this->addItem(strandsceneitems[domain_id]);
            //this->addRect(strandsceneitems[domain_id]->boundingRect());
            rendered_domains.insert(domain_id);
            if (rendered_domains.find(DOMAINS[domain_id]->getBoundId()) != rendered_domains.end()) {
                //the bound_domain has already been rendered, so render the dot as well
                this->addItem(strandsceneitems[-domain_id]);
            }
        }
    }
    setSceneRect(itemsBoundingRect());
}

QRectF StrandScene::combineRects(const QRectF &a, const QRectF &b) const {
    return QRectF(QPointF(std::min(a.topLeft().x(), b.topLeft().x()), std::min(a.topLeft().y(), b.topLeft().y())),
                  QPointF(std::max(a.bottomRight().x(), b.bottomRight().x()), std::max(a.bottomRight().y(), b.bottomRight().y())));
}

QRectF StrandScene::boundingRect(const std::vector<StrandSceneItem*> &items) const {
    QRectF curr_rect;
    bool flag = true;
    for(auto item : items) {
        if (flag) {
            curr_rect = item->boundingRect();
            flag = false;
        } else curr_rect = combineRects(curr_rect, item->boundingRect());
    }
    return curr_rect;
}

void StrandScene::translate(const std::vector<StrandSceneItem*> &items, qreal dx, qreal dy) {
    //translates all items in items by t
    for(auto item : items) {
        item->getPath().translate(dx, dy);
    }
}

void StrandScene::render(const QString &text) {
    std::stringstream ss(text.toStdString());
    std::string type;
    int id;
    ss >> type;
    if (type == "Species") {
        ss >> id;
        renderSpecies(id);
    } else if (type == "Strand") {
        ss >> id;
        renderStrand(id);
    } else {
        //raise exception
    }
}

void StrandScene::clearScene() {
    //clear all items from the scene and reset the curr_pos and prev_pos
    //clear() removes all items from the scene and also deletes them
    //clear();
    for (auto item : this->items()) {
        //this will only remove the item from the scene but won't delete it
        this->removeItem(item);
    }
}

void StrandScene::resetPenPos() {
    curr_pos = QPointF(1, 1);
    prev_pos = QPointF(-1, -1);
}

