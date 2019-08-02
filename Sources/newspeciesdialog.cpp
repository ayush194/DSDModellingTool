#include "Headers/newspeciesdialog.h"
#include "Headers/strand.h"
#include "Headers/species.h"
#include <QDialog>
#include <QGridLayout>
#include <iostream>
#include <sstream>

NewSpeciesDialog::NewSpeciesDialog(QWidget *parent) : AbstractNewDialog(parent) {
    setFixedSize(400, 190);
    strandidslabel = new QLabel(tr("Strand IDs"), this);
    strandidserror = new QLabel(this);
    speciesidslabel = new QLabel(tr("Species IDs"), this);
    speciesidserror = new QLabel(this);
    speciesidslineedit = new QLineEdit(this);
    strandidslineedit = new QLineEdit(this);
    //cancelbutton = new QPushButton(tr("Cancel"), this);
    //okbutton = new QPushButton(tr("OK"), this);

    QGridLayout *mainlayout = new QGridLayout;

    mainlayout->addWidget(strandidslabel, 0, 0);
    mainlayout->addWidget(strandidslineedit, 0, 1);
    mainlayout->addWidget(strandidserror, 1, 1);
    mainlayout->addWidget(speciesidslabel, 2, 0);
    mainlayout->addWidget(speciesidslineedit, 2, 1);
    mainlayout->addWidget(speciesidserror, 3, 1);
    mainlayout->addWidget(cancelbutton, 4, 0);
    mainlayout->addWidget(okbutton, 4, 1);

    this->setLayout(mainlayout);
}

NewSpeciesDialog::~NewSpeciesDialog() {

}

void NewSpeciesDialog::acceptInput() {
    std::istringstream strand_iss(strandidslineedit->text().toStdString()),
            species_iss(speciesidslineedit->text().toStdString());
    std::vector<int> strand_ids, species_ids;
    while(strand_iss.rdbuf()->in_avail()) {
        int id;
        strand_iss >> id;
        //std::cout << id << std::endl;
        strand_ids.push_back(id);
        if (STRANDS.find(id) == STRANDS.end()) {
            std::cerr << "this strand doesn't even exist !" << std::endl;
        } else if (STRANDS[id]->getSpeciesId() != -1) {
            std::cerr << "this strand belongs to another species !" << std::endl;
        }
        //everything has been verified for this strand
    }
    while(species_iss.rdbuf()->in_avail()) {
        int id;
        species_iss >> id;
        //std::cout << id << std::endl;
        species_ids.push_back(id);
        if (SPECIES.find(id) == SPECIES.end()) {
            std::cerr << "this species doesn't even exist !" << std::endl;
        }
        //everything has been verified for this species
    }
    //create a new species and add all strands to it
    for (int species_id : species_ids) {
        for (int strand_id : SPECIES[species_id]->getStrands()) {
            strand_ids.push_back(strand_id);
        }
        //destroy this species
        //SPECIES[species_id]->~Species();
        SPECIES.erase(species_id);
    }
    int new_species_id = dsd::Species::getNewId();
    new dsd::Species(strand_ids, {}, new_species_id);
    emit inputValid(new_species_id);
}
