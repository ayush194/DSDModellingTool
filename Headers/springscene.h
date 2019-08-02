#ifndef SPRINGSCENE_H
#define SPRINGSCENE_H

#include <QObject>
#include <map>
#include "Headers/strand.h"
#include "Headers/domain.h"
#include "Headers/species.h"
#include "Headers/domainitem.h"
#include <QGraphicsScene>

#define DISTANCE 50
#define THRESHOLD 0.00000000001
#define ALPHA 0.00001
typedef std::pair<int, int> pii;

class SpringScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SpringScene(int id, QObject* parent = nullptr);
    //~SpringScene() override;
    void updateParameters();
    void calculateCost();
    void calculateGradient();
    double getCost();
    void setCost(double);
    double costForDomain(int);
    void performGradientDescent();
    void calculateGradForDomain(int);
    //double CMAESCost(const double*, const int&);
    void optimize();

signals:
    void costChanged(double);

private:
    int species_id;
    double cost;
    std::map<int, DomainItem*> domainitems;
    //std::map<pii, SpringItem*> springitems;
};

#endif // SPRINGSCENE_H
