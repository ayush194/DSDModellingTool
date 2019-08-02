//
//  strand.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <unordered_set>
#include <queue>
#include <sstream>
#include <iostream>
#include "Headers/domain.h"
#include "Headers/strand.h"

using namespace dsd;

std::map<int, Strand*> Strand::objects;
int Strand::new_id = 1;

Strand::Strand(const std::vector<std::string>& seqs, const std::vector<int>& ls, const std::vector<int>& ids, int id, int species_id) {
    assert(seqs.size() == ls.size() && ls.size() == ids.size());
    assert(!Strand::exists(id));
    //assert that no other strand exists with the same id
    for(int id : ids) {
        assert(!Domain::exists(id));
        //assert that no other domain exists with the same id
    }
    this->id = id;
    this->species_id = species_id;
    Strand::objects[id] = static_cast<Strand*>(this);
    std::vector<int> tmp;
    for(uint i = 0; i < seqs.size(); i++) {
        Domain* newdomain = new Domain(seqs[i], ls[i], ids[i], id);
        if (i > 0) newdomain->setPrevId(ids[i-1]);
        if (i < ids.size()-1) newdomain->setNextId(ids[i+1]);
        tmp.push_back(ids[i]);
    }
    this->domains = tmp;
    new_id++;
    //findHairpinLoops();
    //findBulgeLoops();
}

Strand::~Strand() {
    for(int domain_id : domains) {
        Domain::getObjects().erase(domain_id);
    }
    Strand::objects.erase(id);
    //all domains inside this strand must be cleared
}
/*
void Strand::addDomains(dsd::Domain& ds, ...) {
    for(dsd::Domain& d : ds) {
        //the list must copy the argument provided to push_back
        domains.push_back(d);
        //d.ofStrand = this;
    }
}
void Strand::addDomains(dsd::Domain ds, ...) {
    for(dsd::Domain& d : ds) {
        //the list must copy the argument provided to push_back
        domains.push_back(d);
        //d.ofStrand = this;
    }
}
*/

void Strand::setSpeciesId(int id) {
    this->species_id = id;
}
int Strand::getSpeciesId() {
    return this->species_id;
}
bool Strand::exists(int id) {
    return Strand::objects.find(id) != Strand::objects.end();
}
std::map<int, Strand*>& Strand::getObjects() {
    return Strand::objects;
}
std::string Strand::getInfo() const {
    std::stringstream ss;
    ss << "Strand(id : ";
    ss << this->id;
    ss << ", domains : [";
    for(int domain_id : this->domains) {
        ss << Domain::getObjects()[domain_id]->getSeq();
        if (Domain::getObjects()[domain_id]->isToehold()) {
            ss << "^";
        }
        ss << " ";
    }
    ss << "])";
    std::string info = ss.str();
    return info;
}
const std::vector<int>& Strand::getDomains() const {
    return this->domains;
}

//types of loops in dna
//internal loop
//bulge loop
//hairpin loop

void Strand::findHairpinLoops() {
    //finds and marks all hairpin loops in the strand
    bool self_binding_started = false;
    bool hairpin_loop_started = false;
    int curr_domain_id = getDomains()[0];
    while(curr_domain_id != -1) {
        int bound_domain_id = DOMAINS[curr_domain_id]->getBoundId();
        if (bound_domain_id != -1 && DOMAINS[bound_domain_id]->getStrandId() == this->id) {
            //hairpin loop detected
            self_binding_started = true;
            hairpin_loop_started = false;
        } else if (self_binding_started || hairpin_loop_started) {
            //this and consecutive domains are involved in the loop
            DOMAINS[curr_domain_id]->setShape(hairpin);
            self_binding_started = false;
            hairpin_loop_started = true;
        } else {
            self_binding_started = false;
            hairpin_loop_started = false;
        }
        curr_domain_id = DOMAINS[curr_domain_id]->getNextId();
    }
}

void Strand::findBulgeLoops() {
    //a bulge loop occurs when two non-consecutive domains of this strand are bound to the same strand
    //i am assuming that none of the domains in the bulge is bound
    std::vector<int> potential_bulge;
    std::unordered_set<int> bound_strands;
    int curr_domain_id = getDomains()[0];
    while(curr_domain_id != -1) {
        int bound_domain_id = DOMAINS[curr_domain_id]->getBoundId();
        if (bound_domain_id == -1) {
            potential_bulge.push_back(curr_domain_id);
        } else if (bound_strands.find(DOMAINS[bound_domain_id]->getStrandId()) != bound_strands.end()) {
            //the potential bulge loop is an actual bulge loop
            for(int domain_id : potential_bulge) {
                DOMAINS[domain_id]->setShape(bulge);
            }
            potential_bulge.clear();
        } else {
            if (DOMAINS[bound_domain_id]->getStrandId() != this->id) {
                bound_strands.insert(DOMAINS[bound_domain_id]->getStrandId());
            }
            potential_bulge.clear();
        }
        curr_domain_id = DOMAINS[curr_domain_id]->getNextId();
    }
}

void Strand::findInternalLoops() {
    //an internal loop is basically like two bulge loops on antiparallel strands
}

uint Strand::numberOfStrandsInConnectedComponent(int domain_id) {
    //do a bfs from this domain_id;
    std::queue<int> bfs;
    std::unordered_set<int> visited;
    std::unordered_set<int> strands;
    bfs.push(domain_id);
    while (!bfs.empty()) {
        int top = bfs.front();
        bfs.pop();
        if (visited.find(top) != visited.end()) continue;
        visited.insert(top);
        if (strands.find(DOMAINS[top]->getStrandId()) == strands.end()) {
            strands.insert(DOMAINS[top]->getStrandId());
        }
        //push the neighbouring domains into the queue
        if (DOMAINS[top]->getBoundId() != -1) bfs.push(DOMAINS[top]->getBoundId());
        if (DOMAINS[top]->getNextId() != -1) bfs.push(DOMAINS[top]->getNextId());
        if (DOMAINS[top]->getPrevId() != -1) bfs.push(DOMAINS[top]->getPrevId());
    }
    return strands.size();
}

int Strand::getNewId() {
    return Strand::new_id;
}
