#include "Headers/dsdparser.h"
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

DSDParser::DSDParser() {

}

void DSDParser::parseInput(std::string filename, std::string delimiter) {
    //open the file
    std::cout << "about to start parsing" << std::endl;
    if (!QFile::exists(filename.c_str())) {
        std::cerr << "DSDParser : Input file not found!" << std::endl;
    }
    QFile inputfile(filename.c_str());
    inputfile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&inputfile);
    QString line = in.readLine();
    std::vector<std::string> species_bngl;
    while (!line.isNull()) {
        //std::cout << "hello" << std::endl;
        if (line.toStdString() == delimiter) {
            //end of one species, parse this species and empty vector species_bngl for the next one
            parseInputSpecies(species_bngl);
            species_bngl.clear();
        } else {
            std::cout << line.toStdString() << std::endl;
            species_bngl.push_back(line.toStdString());
        }
        line = in.readLine();
    }
    /*
    if(!inputfile) {
        std::cerr << "DSDParser : File was not opened !" << std::endl;
    }*/
    //std::freopen(filename.c_str(), "r", stdin);
/*
    std::vector<std::string> species_bngl;
    for(std::string line; std::getline(inputfile, line); ) {
        std::cout << "hello" << std::endl;
        std::cout << line << std::endl;
        if (line == delimiter) {
            //end of one species, parse this species and empty vector species_bngl for the next one
            parseSpecies(species_bngl);
            species_bngl.clear();
        } else if (line.find('<') && line.find('>')) {
            species_bngl.push_back(line);
        }
    }
*/
    //break the file into chunks of species data using the delimiter
    inputfile.close();
}

DSDParser::~DSDParser() {}

void DSDParser::parseInputSpecies(std::vector<std::string> &species_bngl) {
    std::cout << "species begins" << std::endl;
    std::map<std::string, int> bonds;
    std::vector<std::pair<int, int>> bound_domains;
    std::vector<int> strand_ids;
    for(std::string line : species_bngl) {
        //reading one line of the species_bngl i.e. one strand of the species
        std::vector<std::string> domain_seqs;
        std::vector<int> domain_lengths;
        std::vector<int> domain_ids;
        //std::cout << line << std::endl;
        std::istringstream iss(line.substr(line.find('<')+1, line.find('>')-line.find('<')-1));
        std::vector<std::string> tokens;
        int new_domain_id = dsd::Domain::getNewId();
        while(iss.rdbuf()->in_avail()) {
            std::string token;
            iss >> token;
            std::string domain_seq;
            if (token.find('!') != std::string::npos) {
                //means this is a bound domain
                domain_seq = token.substr(0, token.find('!'));
                std::string domain_bond = token.substr(token.find('!')+1, token.size() - domain_seq.size()-1);
                if (bonds.find(domain_bond) != bonds.end()) {
                    //the other domain for this bond is bonds[domain_bond]
                    bound_domains.push_back(std::make_pair(new_domain_id, bonds[domain_bond]));
                } else {
                    bonds[domain_bond] = new_domain_id;
                }
            } else {
                //this is a simple unbound domain
                domain_seq = token;

            }
            if (domain_seq.find('^') != std::string::npos) {
                //this is a toehold
                domain_seq = domain_seq.erase(domain_seq.find('^'), 1);
                domain_lengths.push_back(4);
            } else {
                //this is a regular recognition domain
                domain_lengths.push_back(10);
            }
            domain_seqs.push_back(domain_seq);
            domain_ids.push_back(new_domain_id);
            new_domain_id++;
        }
        int new_species_id = dsd::Strand::getNewId();
        for(std::string i : domain_seqs) std::cout << i << ' ';
        std::cout << std::endl;
        for(int i : domain_lengths) std::cout << i << ' ';
        std::cout << std::endl;
        for(int i : domain_ids) std::cout << i << ' ';
        std::cout << std::endl;
        std::cout << new_species_id << std::endl;
        new dsd::Strand(domain_seqs, domain_lengths, domain_ids, new_species_id);
        strand_ids.push_back(new_species_id);
    }
    new dsd::Species(strand_ids, bound_domains, dsd::Species::getNewId());
    std::cout << "species ends" << std::endl;
}

void DSDParser::parseSpecies(std::string filename, std::string delimiter) {
    //this function parses all the species of the system to output.txt in BNGL format
    if (!QFile::exists(filename.c_str())) {
        std::cerr << "DSDParser : Output file not found!" << std::endl;
    }
    QFile outputfile(":/output.txt");
    if (!outputfile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        std::cerr << "File did not open !" << std::endl;
    }
    QTextStream out(&outputfile);
    for(auto it = SPECIES.begin(); it != SPECIES.end(); it++) {
        //parse this species into text
        for(int strand_id : it->second->getStrands()) {
            out << "<";
            for(int domain_id : STRANDS[strand_id]->getDomains()) {
                //print the domainseq for this domain
                std::string domainseq = DOMAINS[domain_id]->getSeq();
                if (DOMAINS[domain_id]->isToehold()) {
                    if (DOMAINS[domain_id]->getSeq().back() == '*') {
                        domainseq = domainseq.insert(domainseq.size()-1, "^");
                    } else {
                        domainseq = domainseq.insert(domainseq.size(), "^");
                    }
                }
                if (DOMAINS[domain_id]->getBoundId() != -1) {
                    //bound to some other domain, need to indicate the bond
                    domainseq += "!i" + std::to_string(std::min(domain_id, DOMAINS[domain_id]->getBoundId()));
                }
                //out << domainseq.c_str();
            }
            //out << ">\n";
        }
        //out << delimiter.c_str() <<"\n";
    }
    outputfile.close();
}
