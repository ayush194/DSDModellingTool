//
//  newstranddialog.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 2/6/19.
//

#include <iostream>
#include <QGridLayout>
#include "Headers/newstranddialog.h"
#include "Headers/strand.h"
#include "Headers/domain.h"
#include <string>
#include <QFont>
#include <sstream>

NewStrandDialog::NewStrandDialog(QWidget *parent) : AbstractNewDialog(parent) {
    setFixedSize(400, 190);
    domainseqlabel = new QLabel(tr("Domain Sequence"), this);
    domainseqlineedit = new QLineEdit(this);
    domainseqerror = new QLabel(this);
    lengthslabel = new QLabel(tr("Lengths"), this);
    lengthslineedit = new QLineEdit(this);
    lengthserror = new QLabel(this);
    //cancelbutton = new QPushButton(tr("Cancel"), this);
    //okbutton = new QPushButton(tr("OK"), this);

    /*
    QFontMetrics m(domainseqtextedit -> font());
    domainseqlineedit->setFixedHeight(m.lineSpacing());
    idtextedit->setFixedHeight(m.lineSpacing());
    */
    QFont warningfont;
    warningfont.setPointSize(9);
    warningfont.setItalic(true);
    lengthserror->setFont(warningfont);
    domainseqerror->setFont(warningfont);

    //position all the child elements
    QGridLayout *mainlayout = new QGridLayout;
    //mainLayout->setColumnStretch(0, 1);
    mainlayout->addWidget(domainseqlabel, 0, 0);
    mainlayout->addWidget(domainseqlineedit, 0, 1);
    mainlayout->addWidget(domainseqerror, 1, 1);
    mainlayout->addWidget(lengthslabel, 2, 0);
    mainlayout->addWidget(lengthslineedit, 2, 1);
    mainlayout->addWidget(domainseqerror, 3, 1);
    mainlayout->addWidget(cancelbutton, 4, 0);
    mainlayout->addWidget(okbutton, 4, 1);

    this->setLayout(mainlayout);
}

NewStrandDialog::~NewStrandDialog() {
    std::cout << "strand window destroyed" << std::endl;
}

void NewStrandDialog::acceptInput() {
    //verify if the given id is a positive integer and is not already taken;
    //verify if the given domain sequence is a space separated list of strings
    //if everything is ok, create a new strand with the given id
    bool flag = true;
    std::istringstream domainseqss(domainseqlineedit->text().toStdString()),
            lengthsss(lengthslineedit->text().toStdString());
    //std::cout << domainseqss.str() << " " << lengthsss.str() << std::endl;
    std::vector<std::string> seqs;
    std::vector<int> lengths;
    //flag = false;
    //iderror->setText(tr("."));
    while (domainseqss.rdbuf()->in_avail()) {
        std::string tmp;
        domainseqss >> tmp;
        std::cout << tmp << std::endl;
        seqs.push_back(tmp);
    }
    while (lengthsss.rdbuf()->in_avail()) {
        int tmp;
        lengthsss >> tmp;
        std::cout << tmp << std::endl;
        lengths.push_back(tmp);
    }
    if (flag) {
        //everything has been verified;
        //create a new strand with the given info and close the dialog window
        std::vector<int> ids;
        int next_empty_id = dsd::Domain::getObjects().size()+1;
        for(int i = next_empty_id; i < next_empty_id + seqs.size(); i++) {
            ids.push_back(i);
        }
        std::cout << seqs.size() << ' ' << lengths.size() << ' ' << ids.size() << std::endl;
        int strand_id = dsd::Strand::getObjects().size()+1;
        new dsd::Strand(seqs, lengths, ids, strand_id);
        emit inputValid(strand_id);
        //change the dropdown list
    }
}

