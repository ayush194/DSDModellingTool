//
//  mainwindow.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 30/5/19.
//

#include "Headers/mainwindow.h"
#include "Headers/strandscene.h"
#include "Headers/newstranddialog.h"
#include "Headers/newspeciesdialog.h"
#include "Headers/newduplexdialog.h"
#include "Headers/domain.h"
#include "Headers/strand.h"
#include "Headers/species.h"
#include <QIcon>
#include <QApplication>
#include <QtWidgets>
#include <iostream>
#include <sstream>
#include <string>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    //Set size of the window
    //setFixedSize(400, 400);

    //initialize the sub-components
    maindropdown = new QComboBox(this);
    //strandscene = new StrandScene();
    newspeciesbutton = new QPushButton(tr("New Species"), this);
    newstrandbutton = new QPushButton(tr("New Strand"), this);
    newduplexbutton = new QPushButton(tr("New Duplex"), this);
    joinbutton = new QPushButton(QIcon(":/Icons/join.png"), tr("Join"), this);
    unjoinbutton = new QPushButton(QIcon(":/Icons/unjoin.png"), tr("Unjoin"), this);
    foobutton = new QPushButton(tr("foo"), this);
    strandscene = new StrandScene(this);
    springscene = new SpringScene(9, this);
    dashboardscene = new DashboardScene(this);
    graphicsview = new QGraphicsView(strandscene, this);
    //graphicsview->setInteractive(true);
    //graphicsview->setMouseTracking(true);
    newstranddialog = new NewStrandDialog(this);
    newspeciesdialog = new NewSpeciesDialog(this);
    newduplexdialog = new NewDuplexDialog(this);
    costlabel = new QLabel(tr("Cost : "), this);

    //position all the child elements
    QGridLayout *mainlayout = new QGridLayout;
    mainlayout->setColumnStretch(0, 1);
    mainlayout->addWidget(maindropdown, 0, 0);
    mainlayout->addWidget(newspeciesbutton, 2, 0);
    mainlayout->addWidget(newstrandbutton, 4, 0);
    mainlayout->addWidget(newduplexbutton, 6, 0);
    mainlayout->addWidget(joinbutton, 8, 0);
    mainlayout->addWidget(unjoinbutton, 10, 0);
    mainlayout->addWidget(foobutton, 12, 0);
    //mainLayout->addWidget(renderarea, 0, 2, 9, 1);
    mainlayout->addWidget(graphicsview, 0, 2, 11, 1);
    mainlayout->addWidget(costlabel, 12, 2);
    setLayout(mainlayout);

    graphicsview->setFixedSize(400, 300);
    graphicsview->show();

    refreshMainDropdown();

    //connect the clicked signals of the push buttons to the appropriate slots
    QObject::connect(newstrandbutton, SIGNAL(clicked()), newstranddialog, SLOT(exec()));
    QObject::connect(newspeciesbutton, SIGNAL(clicked()), newspeciesdialog, SLOT(exec()));
    QObject::connect(newduplexbutton, SIGNAL(clicked()), newduplexdialog, SLOT(exec()));
    QObject::connect(maindropdown, SIGNAL(activated(const QString&)), this, SLOT(dropdownActiveItemChanged(const QString&)));
    //QObject::connect(maindropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), maindropdown, SLOT(setCurrentIndex(int)));
    QObject::connect(newstranddialog, SIGNAL(inputValid(int)), this, SLOT(refreshMainDropdown()));
    QObject::connect(newstranddialog, SIGNAL(inputValid(int)), strandscene, SLOT(createStrand(int)));
    QObject::connect(newspeciesdialog, SIGNAL(inputValid(int)), this, SLOT(refreshMainDropdown()));
    QObject::connect(newspeciesdialog, SIGNAL(inputValid(int)), strandscene, SLOT(createSpecies(int)));
    QObject::connect(newduplexdialog, SIGNAL(inputValid(int)), this, SLOT(refreshMainDropdown()));
    QObject::connect(newduplexdialog, SIGNAL(inputValid(int)), strandscene, SLOT(createSpecies(int)));
    QObject::connect(joinbutton, SIGNAL(clicked()), this, SLOT(joinDomains()));
    QObject::connect(unjoinbutton, SIGNAL(clicked()), this, SLOT(unjoinDomains()));
    QObject::connect(springscene, SIGNAL(costChanged(double)), this, SLOT(updateCost(double)));
    QObject::connect(foobutton, SIGNAL(clicked()), this, SLOT(foo()));

    int idx = maindropdown->findData(tr("SpringScene"));
    maindropdown->setCurrentIndex(idx);
    emit maindropdown->activated("SpringScene");
    //springscene->performGradientDescent();
}

void MainWindow::addItemInDropdown(int id) {
    QString tmp(tr("Strand "));
    for (char c : std::to_string(id)) tmp.append(c);
    maindropdown->addItem(tmp, QVariant::fromValue<int>(id));
    //strandscene->createStrand(id);
}

void MainWindow::joinDomains() {
    std::cout << "joindomains" << std::endl;
    //get all the selected items;
    std::cout << strandscene->selectedItems().size() << std::endl;
    int id1 = -1, id2 = -1;
    bool joined = true;
    for (auto item : strandscene->selectedItems()) {
        if (!joined) {
            id2 = static_cast<StrandSceneItem*>(item)->getDomainId();
            std::cout << id1 << ' ' << id2 << std::endl;
            dsd::Domain::bind(id1, id2);
            //need to identify if the two domains belong to different strands or the same one or different species or the same one
            //based on that a dialog should open up for creation of a new species
            //right now we assume they belong to the same species
            joined = true;
        } else {
            id1 = static_cast<StrandSceneItem*>(item)->getDomainId();
            joined = false;
        }
    }
    //render this species;
    if (id1 != -1) {
        int species_id = STRANDS[DOMAINS[id1]->getStrandId()]->getSpeciesId();
        std::cout << species_id << std::endl;
        strandscene->createSpecies(species_id);
        strandscene->renderSpecies(species_id);
    }
}

void MainWindow::unjoinDomains() {
    int species_id = -1;
    for (auto item : strandscene->selectedItems()) {
        int domain_id = static_cast<StrandSceneItem*>(item)->getDomainId();
        DOMAINS[domain_id]->unbind();
        species_id = STRANDS[DOMAINS[domain_id]->getStrandId()]->getSpeciesId();
    }
    if (species_id != -1) {
        strandscene->createSpecies(species_id);
        strandscene->renderSpecies(species_id);
    }
}

void MainWindow::refreshMainDropdown() {
    //remove all items from the dropdown and insert them all again
    maindropdown->clear();
    maindropdown->addItem("Dashboard");
    for(auto it = STRANDS.begin(); it != STRANDS.end(); it++) {
        //if a strand is not part of a species, it will be shown as an isolated strand in the list
        if (it->second->getSpeciesId() == -1) {
            QString tmp(tr("Strand "));
            for (char c : std::to_string(it->first)) tmp.append(c);
            maindropdown->addItem(tmp, QVariant::fromValue<int>(it->first));
        }
    }
    for(auto it = SPECIES.begin(); it != SPECIES.end(); it++) {
        std::cout << it->first << std::endl;
        QString tmp(tr("Species "));
        for (char c : std::to_string(it->first)) tmp.append(c);
        maindropdown->addItem(tmp, QVariant::fromValue<int>(it->first));
    }
    maindropdown->addItem("SpringScene", tr("SpringScene"));
}

void MainWindow::dropdownActiveItemChanged(const QString& itemname) {
    if (itemname == QString("Dashboard")) {
        graphicsview->setScene(dashboardscene);
    } else if (itemname == QString("SpringScene")) {
        graphicsview->setScene(springscene);
    } else {
        graphicsview->setScene(strandscene);
        strandscene->render(itemname);
    }
}

void MainWindow::updateCost(double cost) {
    //std::cout << "hello" << std::endl;
    std::ostringstream ss;
    ss << cost;
    QString tmp(tr("Cost : "));
    //std::cout << ss.str() << std::endl;
    for(char c : std::to_string(cost)) tmp.append(c);
    //std::cout << "hello" << std::endl;
    //this->costlabel->setText(tmp);
}

void MainWindow::foo() {
    springscene->optimize();
}



