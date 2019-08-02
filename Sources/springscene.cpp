#include "Headers/strand.h"
#include "Headers/domain.h"
#include "Headers/species.h"
#include "Headers/springscene.h"
#include "Headers/domainitem.h"
#include "Headers/mainwindow.h"
#include "cmaes.h"
#include <cmath>
#include <iostream>

#define DELTA_I_X1_BOUND_X1 (d->line.x1() - d_bound->line.x1())
#define DELTA_I_Y1_BOUND_Y1 (d->line.y1() - d_bound->line.y1())
#define DELTA_I_X2_BOUND_X2 (d->line.x2() - d_bound->line.x2())
#define DELTA_I_Y2_BOUND_Y2 (d->line.y2() - d_bound->line.y2())
//#define DELTA_I_X2_BOUND_X2 (d->line.x1() + LENGTH*cos(d->line.angle()) - d_bound->line.x1() - LENGTH*cos(d_bound->line.angle()))
//#define DELTA_I_Y2_BOUND_Y2 (d->line.y1() + LENGTH*sin(d->line.angle()) - d_bound->line.y1() - LENGTH*sin(d_bound->line.angle()))

//#define DELTA_I_X1_BOUND_X2 (d->line.x1() - d_bound->line.x2() - LENGTH*cos(d_bound->line.angle()))
//#define DELTA_I_Y1_BOUND_Y2 (d->line.y1() - d_bound->line.y2() - LENGTH*sin(d_bound->line.angle()))
//#define DELTA_I_X2_BOUND_X1 (d->line.x1() + LENGTH*cos(d->line.angle()) - d_bound->line.x1())
//#define DELTA_I_Y2_BOUND_Y1 (d->line.y1() + LENGTH*sin(d->line.angle()) - d_bound->line.y1())
//#define DELTA_I_X2_NEXT_X1 (d->line.x1() + LENGTH*cos(d->line.angle()) - d_next->line.x1())
//#define DELTA_I_Y2_NEXT_Y1 (d->line.y1() + LENGTH*sin(d->line.angle()) - d_next->line.y1())
//#define DELTA_PREV_X2_I_X1 (d_prev->line.x1() + LENGTH*cos(d->line.angle()) - d->line.x1())
//#define DELTA_PREV_Y2_I_Y1 (d_prev->line.y1() + LENGTH*sin(d->line.angle()) - d->line.y1())

#define DELTA_I_X1_BOUND_X2 (d->line.x1() - d_bound->line.x2())
#define DELTA_I_Y1_BOUND_Y2 (d->line.y1() - d_bound->line.y2())
#define DELTA_I_X2_BOUND_X1 (d->line.x2() - d_bound->line.x1())
#define DELTA_I_Y2_BOUND_Y1 (d->line.y2() - d_bound->line.y1())
#define DELTA_I_X2_NEXT_X1 (d->line.x2() - d_next->line.x1())
#define DELTA_I_Y2_NEXT_Y1 (d->line.y2() - d_next->line.y1())
#define DELTA_PREV_X2_I_X1 (d_prev->line.x2() - d->line.x1())
#define DELTA_PREV_Y2_I_Y1 (d_prev->line.y2() - d->line.y1())

SpringScene::SpringScene(int id, QObject *parent) : QGraphicsScene(parent) {
    //traverse the strand graph and add all the domains to this scene
    this->species_id = id;
    for(int strand_id : SPECIES[id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            this->domainitems[domain_id] = new DomainItem(domain_id);
            this->addItem(domainitems[domain_id]);
        }
    }
}

double SpringScene::costForDomain(int id) {
    DomainItem* d = domainitems[id];
    double c = 0.0;
    //calculate the cost for this domain
    if(DOMAINS[id]->getBoundId() != -1) {
        DomainItem* d_bound = domainitems[DOMAINS[id]->getBoundId()];
        //c += pow(pow(pow(d->line.x1() - d_bound->line.x1(), 2) + pow(d->line.y1() - d_bound->line.y1(), 2), 0.5) - DISTANCE, 2) +
        //        pow(pow(pow(d->line.x1() + LENGTH*cos(d->line.angle() - d_bound->line.x1() - LENGTH*cos(d_bound->line.angle())), 2) +
        //                pow(d->line.y1() + LENGTH*sin(d->line.angle() - d_bound->line.y1() - LENGTH*sin(d_bound->line.angle())), 2), 0.5) - DISTANCE, 2)
        c += (pow(pow(pow(DELTA_I_X1_BOUND_X1, 2) + pow(DELTA_I_Y1_BOUND_Y1, 2), 0.5) - DISTANCE, 2) +
             pow(pow(pow(DELTA_I_X2_BOUND_X2, 2) + pow(DELTA_I_Y2_BOUND_Y2, 2), 0.5) - DISTANCE, 2));
        /*
        //cost of this spring
        //first term is for the euclidean distance between the bound domains
        c += 0.5 * pow(pow(pow(d->line.x1() - d_bound->line.x1(), 2) + pow(d->line.y1()-d_bound->line.y1(), 2), 0.5) - DISTANCE, 2);
        //second term is the cosine of the angle between them, this is to ensure that they remain antiparallel
        */
        c += 4000*(std::cos((d->line.angle() - d_bound->line.angle()) * M_PI / 180.0) + 1);
    }
    if (DOMAINS[id]->getNextId() != -1) {
        DomainItem* d_next = domainitems[DOMAINS[id]->getNextId()];
        //cost for this domain and next_domain to stay connected
        //c += pow((d->line.x1() + LENGTH*cos(d->line.angle()) - d_next->line.x1()), 2) +
        //        pow((d->line.y1() + LENGTH*sin(d->line.angle()) - d_next->line.y1()), 2);
        c += (pow(DELTA_I_X2_NEXT_X1, 2) + pow(DELTA_I_Y2_NEXT_Y1, 2));
    }
    return c;
}

void SpringScene::calculateGradForDomain(int id) {
    //calculate grad.c(i)
    //calculate δc/δdi.x
    double delx = 0.0, dely = 0.0, deltheta =0.0;
    DomainItem *d = domainitems[id];
    if (DOMAINS[id]->getPrevId() != -1) {
        DomainItem *d_prev = domainitems[DOMAINS[id]->getPrevId()];
        delx += -2*DELTA_PREV_X2_I_X1;
    }
    if (DOMAINS[id]->getBoundId() != -1) {
        DomainItem *d_bound = domainitems[DOMAINS[id]->getBoundId()];
        delx += 2*2*(DELTA_I_X1_BOUND_X1)*(1 - 2*DISTANCE/pow(pow(DELTA_I_X1_BOUND_X1, 2) + pow(DELTA_I_Y1_BOUND_Y1, 2), 0.5)) +
                2*2*(DELTA_I_X2_BOUND_X2)*(1 - 2*DISTANCE/pow(pow(DELTA_I_X2_BOUND_X2, 2) + pow(DELTA_I_Y2_BOUND_Y2, 2), 0.5));
    }
    /*
    if (DOMAINS[id]->getBoundId() != -1) {
        DomainItem *d_bound = domainitems[DOMAINS[id]->getBoundId()];
        //first term
        delx += (2 * (d->line.x1() - d_bound->line.x1())) - 2 * DISTANCE * (d->line.x1() - d_bound->line.x1()) /
                       pow(pow(d->line.x1() - d_bound->line.x1(), 2) + pow(d->line.y1() - d_bound->line.y1(), 2), 0.5);
    }
    */
    if (DOMAINS[id]->getNextId() != -1) {
        DomainItem *d_next = domainitems[DOMAINS[id]->getNextId()];
        delx += 2*(DELTA_I_X2_NEXT_X1);
    }

    //calculate δc/δdi.y
    if (DOMAINS[id]->getPrevId() != -1) {
        DomainItem *d_prev = domainitems[DOMAINS[id]->getPrevId()];
        dely += -2*DELTA_PREV_Y2_I_Y1;
    }

    if (DOMAINS[id]->getBoundId() != -1) {
        DomainItem *d_bound = domainitems[DOMAINS[id]->getBoundId()];
        //first term
        dely += 2*2*(DELTA_I_Y1_BOUND_Y1)*(1 - 2*DISTANCE/pow(pow(DELTA_I_X1_BOUND_X1, 2) + pow(DELTA_I_Y1_BOUND_Y1, 2), 0.5)) +
                2*2*(DELTA_I_Y2_BOUND_Y2)*(1 - 2*DISTANCE/pow(pow(DELTA_I_X2_BOUND_X2, 2) + pow(DELTA_I_Y2_BOUND_Y2, 2), 0.5));
    }
    if (DOMAINS[id]->getNextId() != -1) {
        DomainItem *d_next = domainitems[DOMAINS[id]->getNextId()];
        dely += 2*(DELTA_I_Y2_NEXT_Y1);
    }


    //calculate δc/δdi.θ
    if (DOMAINS[id]->getPrevId() != -1) {
        DomainItem *d_prev = domainitems[DOMAINS[id]->getPrevId()];
        //this term is 0
    }
    if (DOMAINS[id]->getBoundId() != -1) {
        DomainItem *d_bound = domainitems[DOMAINS[id]->getBoundId()];
        //first term
        deltheta += 2*2*(-DELTA_I_X2_BOUND_X2*LENGTH*sin(d->line.angle()) + DELTA_I_Y2_BOUND_Y2*LENGTH*cos(d->line.angle())) *
                (1 - 2*DISTANCE/pow(pow(DELTA_I_X2_BOUND_X2, 2) + pow(DELTA_I_Y2_BOUND_Y2, 2), 0.5));
        deltheta += -2*sin(d->line.angle() - d_bound->line.angle());
    }
    if (DOMAINS[id]->getNextId() != -1) {
        DomainItem *d_next = domainitems[DOMAINS[id]->getNextId()];
        //this is actually the second term from the above scope
        deltheta += 2*(-DELTA_I_X2_NEXT_X1*LENGTH*sin(d->line.angle()) + DELTA_I_Y2_NEXT_Y1*LENGTH*cos(d->line.angle()));
        //deltheta += 2 * (d->line.x1() + LENGTH * cos(d->line.angle()) - d_next->line.x1()) * (-LENGTH * sin(d->line.angle())) +
        //        2 * (d->line.y1() + LENGTH * sin(d->line.angle()) - d_next->line.y1()) * (LENGTH * cos(d->line.angle()));
    }
    //std::cout << "gradient : " << delx << ' ' << dely << ' ' << deltheta << std::endl;
    domainitems[id]->setGrad(delx, dely, deltheta);
}

void SpringScene::calculateCost() {
    //cacluate the cost and set the cost as the new calculated cost
    double c = 0.0;
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            //calculate c(i)
            c += costForDomain(domain_id);
        }
    }
    setCost(c);
}

void SpringScene::calculateGradient() {
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            //calculate grad.c(i)
            calculateGradForDomain(domain_id);
            /*
            double delx, dely, deltheta;
            std::tie(delx, dely, deltheta) = gradForDomain(domain_id);
            domainitems[domain_id]->setGrad(delx, dely, deltheta);
            */
        }
    }
}

void SpringScene::updateParameters() {
    //move in the direction opposite to the gradient at this point
    //update the positions and directions of all the domains
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            domainitems[domain_id]->updateLine(ALPHA);
        }
    }
}

double SpringScene::getCost() {
    return this->cost;
}

void SpringScene::setCost(double new_cost) {
    this->cost = new_cost;
    static_cast<MainWindow*>(this->parent())->updateCost(this->cost);
    //std::cout << "about to emit" << std::endl;
    //emit costChanged(this->cost);
    //std::cout << "emitted" << std::endl;
    //cost changed
}

/*
double SpringScene::CMAESCost(const double *x, const int N) {
    double c = 0.0;
    std::vector<int> domains;
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            domains.push_back(domain_id);
        }
    }
    for (uint i = 0; i < domains.size(); i++) {
        domainitems[domains[i]]->setLine(x[3*i], x[3*i+1], x[3*i+2]);
    }
    for (uint i = 0; i < domains.size(); i++) {
        c += costForDomain(domains[i]);
    }
    return c;
}
*/

void SpringScene::optimize() {
    std::vector<int> domains;
    for(int strand_id : SPECIES[this->species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            domains.push_back(domain_id);
        }
    }
    libcmaes::FitFunc fsphere = [&](const double *x, const int N) {
            double c = 0.0;
            for (uint i = 0; i < domains.size(); i++) {
                std::cout << x[3*i] << ' ' << x[3*i+1] << ' ' << x[3*i+2] << std::endl;
                domainitems[domains[i]]->setLine(x[3*i], x[3*i+1], x[3*i+2]);
            }
            for (uint i = 0; i < domains.size(); i++) {
                c += costForDomain(domains[i]);
            }
            return c;
        };
    uint nd = 0;
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            nd += 1;
        }
    }
    //if there are nd domains then there are 3*nd parameters
    int dim = 3*nd; // problem dimensions.
    double sigma = 0.1; // initial step-size, i.e. estimated initial parameter error.
    std::vector<double> x0(dim,10.0);
    libcmaes::CMAParameters<> cmaparams(x0,sigma);
    cmaparams.set_algo(aCMAES);
    //cmaparams.set_max_fevals(10000); // limits the number of function evaluations to 10000
    //cmaparams.set_max_iter(100000); // limits the number of iterations of the algorithms to 1000000
    //cmaparams.set_ftarget(1e-20); // stops the optimization whenever the objective function values gets below 1e-8

    libcmaes::CMASolutions cmasols = libcmaes::cmaes<>(fsphere,cmaparams);
    std::cout << "best solution: " << cmasols << std::endl;
    std::cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds\n";
    std::cout << cmasols.run_status() << std::endl; // the optimization status, failed if < 0

    const double* params = cmasols.best_candidate().get_x_ptr();
    for(uint i = 0; i < domains.size(); i++) {
        //std::cout << params[3*i] << ' ' << params[3*i+1] << ' ' << params[3*i+2] << std::endl;
        //domainitems[domains[i]]->setLine(params[3*i], params[3*i+1], params[3*i+2]);
        std::cout << domainitems[domains[i]]->line.x1() << ' ' << domainitems[domains[i]]->line.y1() << ' ' << domainitems[domains[i]]->line.x2() << ' ' << domainitems[domains[i]]->line.y2() << ' ' << domainitems[domains[i]]->line.angle() << std::endl;
    }
    double c = 0.0;
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            c += costForDomain(domain_id);
        }
    }
    std::cout << "the cost is " << c << std::endl;
}

void SpringScene::performGradientDescent() {
    //random initialization
    for(int strand_id : SPECIES[species_id]->getStrands()) {
        for(int domain_id : STRANDS[strand_id]->getDomains()) {
            domainitems[domain_id]->randomlyInitialize();
        }
    }
    double diff = 1000;
    calculateCost();
    while (diff > THRESHOLD) {
        //perform another iteration
        double prev_cost = getCost();
        calculateGradient();
        updateParameters();
        this->update();
        calculateCost();
        double new_cost = getCost();
        diff = fabs(prev_cost - new_cost);
        std::cout << prev_cost << ' ' << new_cost << std::endl;
        if (new_cost < 10) break;
    }
    //if the diff is less than the threshold, then stop !
}
