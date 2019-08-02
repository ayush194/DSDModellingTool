#include "Headers/domain.h"
#include "Headers/strand.h"
#include "Headers/species.h"
#include "Headers/newduplexdialog.h"
#include <sstream>
#include <iostream>

NewDuplexDialog::NewDuplexDialog(QWidget *parent) : NewStrandDialog(parent) {}

NewDuplexDialog::~NewDuplexDialog() {}

void NewDuplexDialog::acceptInput() {
    //this is the same as the acceptInput() for NewStrandDialog() with the addition
    //that in this case a species has to be created with the two complementary strands
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
        seqs.push_back(tmp);
    }
    while (lengthsss.rdbuf()->in_avail()) {
        int tmp;
        lengthsss >> tmp;
        lengths.push_back(tmp);
    }
    if (flag) {
        //everything has been verified;
        //create a new strand with the given info and close the dialog window
        std::vector<int> ids1, ids2;
        int i = dsd::Domain::getNewId();
        for(; i < dsd::Domain::getNewId() + seqs.size(); i++) {
            ids1.push_back(i);
        }
        for(; i < dsd::Domain::getNewId() + 2*seqs.size(); i++) {
            ids2.push_back(i);
        }
        std::vector<std::string> compl_seqs(seqs);
        std::vector<int> compl_lengths(lengths);
        std::reverse(compl_lengths.begin(), compl_lengths.end());
        std::reverse(compl_seqs.begin(), compl_seqs.end());
        for(std::string &seq : compl_seqs) {
            if (seq.back() == '*') seq = seq.substr(0, seq.size()-1);
            else seq += '*';
        }
        int strand_id1 = dsd::Strand::getNewId();
        new dsd::Strand(seqs, lengths, ids1, strand_id1);
        int strand_id2 = dsd::Strand::getNewId();
        new dsd::Strand(compl_seqs, compl_lengths, ids2, strand_id2);
        for(uint i = 0; i < ids1.size(); i++) {
            dsd::Domain::bind(ids1[i], ids2[ids2.size()-1-i]);
        }
        int new_species_id = dsd::Species::getNewId();
        std::cout << "new_species_id" << ' ' << new_species_id << std::endl;
        new dsd::Species({strand_id1, strand_id2}, {}, new_species_id);
        std::cout << "emitting new species id" << std::endl;
        emit inputValid(new_species_id);
    }
}
