//
//  main.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <iostream>
#include <QApplication>
#include <QComboBox>
#include <unistd.h>
#include "Headers/domain.h"
#include "Headers/strand.h"
#include "Headers/species.h"
#include "Headers/mainwindow.h"
#include "Headers/dsdparser.h"
#include "cmaes.h"

/*
using namespace libcmaes;

double foo (const double *x, const int N){
  double val = 0.0;
  for (int i=0;i<N;i++)
    val += x[i]*x[i];
  return val;
}

FitFunc fsphere = &foo;
*/

int main(int argc, char **argv) {
    std::srand(time(NULL));
    dsd::Strand a1({"X*", "A*"}, {10, 10}, {1, 2}, 1),
            a2({"T1*", "X", "R"}, {4, 10, 10}, {3, 4, 5}, 2),
            a3({"L", "T2", "X*", "T1"}, {10, 4, 10, 4}, {6, 7, 8, 9}, 3),
            a4({"X", "T2*"}, {10, 4}, {10, 11}, 4),
            a5({"A"}, {10}, {12}, 5);
    dsd::Species A({1, 2, 3, 4, 5}, {{1, 4}, {2, 12}, {3, 9}, {7, 11}, {8, 10}}, 1);

    dsd::Strand b1({"T1*", "X", "R"}, {4, 10, 10}, {13, 14, 15}, 6),
            b2({"X*", "A*"}, {10, 10}, {16, 17}, 7),
            b3({"A"}, {10}, {18}, 8);
    dsd::Species B({6, 7, 8}, {{14, 16}, {17, 18}}, 2);

    dsd::Strand c1({"L", "T2", "X*", "T1"}, {10, 4, 10, 4}, {19, 20, 21, 22}, 9),
            c2({"A", "X", "T2*"}, {10, 10, 4}, {23, 24, 25}, 10);
    dsd::Species C({9, 10}, {{21, 24}, {20, 25}}, 3);

    dsd::Strand d1({"T1*", "X", "R"}, {4, 10, 10}, {26, 27, 28}, 11),
            d2({"L", "T2", "X*", "T1"}, {10, 4, 10, 4}, {29, 30, 31, 32}, 12),
            d3({"X*", "A*"}, {10, 10}, {33, 34}, 13),
            d4({"A", "X", "T2*"}, {10, 10, 4}, {35, 36, 37}, 14);
    dsd::Species D({11, 12, 13, 14}, {{26, 32}, {27, 31}, {33, 36}, {34, 35}, {30, 37}}, 4);

    dsd::Strand e1({"h", "tx", "x"}, {10, 4, 10}, {38, 39, 40}, 15),
            e2({"x", "ta"}, {10, 4}, {41, 42}, 16),
            e3({"a", "ty", "y"}, {10, 4, 10}, {43, 44, 45}, 17),
            e4({"ty*", "a*", "ta*", "x*", "tx*"}, {4, 10, 4, 10, 4}, {46, 47, 48, 49, 50}, 18);
    dsd::Species E({15, 16, 17, 18}, {{39, 50}, {40, 49}, {42, 48}, {43, 47}, {44, 46}}, 5);

    dsd::Strand f1({"ab", "xz", "ce", "df"}, {10, 10, 10, 10}, {51, 52, 53, 54}, 19),
            f2({"ab*", "bg", "kf", "df*"}, {10, 10, 10, 10}, {55, 56, 57, 58}, 20);
    dsd::Species F({19, 20}, {{51, 55}, {54, 58}}, 6);


    dsd::Strand g1({"ace", "jack", "king", "queen", "sap", "jack*"}, {4, 4, 4, 4, 4, 4}, {59, 60 ,61, 62, 63, 64}, 21);
    dsd::Species G({21}, {{60, 64}}, 7);

    dsd::Strand h1({"a", "b", "c", "d"}, {4, 4, 4, 4}, {65, 66, 67, 68}, 22),
            h2({"d*", "a*"}, {4, 4}, {69, 70}, 23);
    dsd::Species H({22, 23}, {{65, 70}, {68, 69}}, 8);

    dsd::Strand i1({"a"}, {4}, {71}, 24),
            i2({"a*"}, {4}, {72}, 25);
    dsd::Species I({24, 25}, {{71, 72}}, 9);

    dsd::Strand j1({"ay", "us", "hk", "um"}, {4, 4, 4, 4}, {73, 74, 75, 76}, 26),
            j2({"um*", "an", "gm", "ay*"}, {4, 4, 4, 4}, {77, 78, 79, 80}, 27);
    dsd::Species J({26, 27}, {{73, 80}, {76, 77}}, 10);

    dsd::Strand k1({"a", "b", "c", "d", "e"}, {4, 4, 4, 4, 4}, {81, 82, 83, 84, 85}, 28),
            k2({"e*", "d*", "f", "g", "h"}, {4, 4, 4, 4, 4}, {86, 87, 88, 89, 90}, 29),
            k3({"h*", "g*", "i", "j", "b*", "a*"}, {4, 4, 4, 4, 4, 4}, {91, 92, 93, 94, 95, 96}, 30);
    dsd::Species K({28, 29, 30}, {{85, 86}, {84, 87}, {90, 91}, {89, 92}, {95, 82}, {96, 81}}, 11);

    dsd::Strand l1({"a", "z"}, {4, 4}, {97, 98}, 31),
            l2({"z*", "a*"}, {4, 4}, {99, 100}, 32),
            l3({"a", "a*"}, {4, 4}, {101, 102}, 33);
    dsd::Species L({31, 32, 33}, {{98, 99}, {100, 101}, {102, 97}}, 12);

    dsd::Strand m1({"a", "b"}, {4, 4}, {103, 104}, 34),
            m2({"a*"}, {4}, {105}, 35);
    dsd::Species M({34, 35}, {{103, 105}}, 13);

    for(int domain_id : STRANDS[21]->getDomains()) {
        switch(DOMAINS[domain_id]->getShape()) {
        case dsd::straight : {std::cout << "straight" << ' '; break;}
        case dsd::hairpin : {std::cout << "hairpin" << ' '; break;}
        case dsd::bulge : {std::cout << "bulge" << ' '; break;}
        case dsd::branch : {std::cout << "branch" << ' '; break;}
        case dsd::internal : {std::cout << "internal" << ' '; break;}
        }
    }
    std::cout << std::endl;
    for(int domain_id : STRANDS[22]->getDomains()) {
        switch(DOMAINS[domain_id]->getShape()) {
        case dsd::straight : {std::cout << "straight" << ' '; break;}
        case dsd::hairpin : {std::cout << "hairpin" << ' '; break;}
        case dsd::bulge : {std::cout << "bulge" << ' '; break;}
        case dsd::branch : {std::cout << "branch" << ' '; break;}
        case dsd::internal : {std::cout << "internal" << ' '; break;}
        }
    }
    std::cout << std::endl;

    DSDParser myparser;
    myparser.parseInput(":/input.txt", "//");
    myparser.parseSpecies(":/output.txt", "//");
    //std::cout << a.getInfo() << std::endl;
    //std::cout << b.getInfo() << std::endl;
    //std::cout << c.getInfo() << std::endl;
    /*
    std::cout << dsd::Domain::getObjects().size() << std::endl;
    for(int i = 1; i <= 12; i++) {
        std::cout << dsd::Domain::getObjects()[i]->getInfo() << std::endl;
    }
    std::cout << dsd::Strand::getObjects().size() << std::endl;
    for(int i = 1; i <= 5; i++) {
        std::cout << dsd::Strand::getObjects()[i]->getInfo() << std::endl;
    }
    std::cout << dsd::Species::getObjects().size() << std::endl;
    for(int i = 1; i <= 1; i++) {
        std::cout << dsd::Species::getObjects()[i]->getInfo() << std::endl;
    }
    */
    /*
    int dim = 10; // problem dimensions.
    double sigma = 0.1; // initial step-size, i.e. estimated initial parameter error.
    std::vector<double> x0(dim,10.0);
    CMAParameters<> cmaparams(x0,sigma);
    cmaparams.set_algo(aCMAES);
    cmaparams.set_max_fevals(10000); // limits the number of function evaluations to 10000
    cmaparams.set_max_iter(100000); // limits the number of iterations of the algorithms to 1000000
    cmaparams.set_ftarget(1e-8); // stops the optimization whenever the objective function values gets below 1e-8

    CMASolutions cmasols = cmaes<>(fsphere,cmaparams);
    std::cout << "best solution: " << cmasols << std::endl;
    std::cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds\n";
    std::cout << cmasols.run_status(); // the optimization status, failed if < 0
    */
    QApplication app (argc, argv);
    MainWindow mainwindow(nullptr);
    mainwindow.show();
    /*
    std::cout << "about to sleep" << std::endl;
    usleep(10000000);
    std::cout << "awake" << std::endl;
    */
    return app.exec();
}
