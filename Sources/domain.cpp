//
//  domain.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <sstream>
#include <iostream>
#include "Headers/domain.h"
#include "Headers/strand.h"

#define TOEHOLD_LIMIT 9

using namespace dsd;

std::map<int, Domain*> Domain::objects;
int Domain::new_id = 1;

Domain::Domain(const std::string& s, int l, int id, int sid) : seq(s), length(l), bound_id(-1), strand_id(sid) {
    auto isAlphaNum = [](std::string s) {for(char c : s) {if (!isalnum(c)) return false;} return true;};
    assert(isAlphaNum(s) || (s.back() == '*' && isAlphaNum(s.substr(0, s.size()-1))));
    //assert(s contains alphanumeric characters with an optional '*' at the end)
    assert(!Domain::exists(id));
    //assert that no Domain object exists with the given id
    assert(Strand::exists(strand_id));
    //assert that a Strand object exists with the given strand_id
    this->id = id;
    this->prev_id = -1;
    this->next_id = -1;
    this->shape = straight;
    Domain::objects[id] = static_cast<Domain*>(this);
    new_id++;
}



Domain::~Domain() {
    Domain::objects.erase(id);
}

bool Domain::isToehold() const {
    //in our current definition of toeholds, sequences of length <= 9 nucleoties are toeholds
    return this->length <= TOEHOLD_LIMIT;
}

bool Domain::isComplement(const Domain& d) const {
    return this->isComplement(d.seq) && this->length == d.length;
}
bool Domain::isComplement(const std::string& s) const {
    return (this->seq.back() == '*' && this->seq.substr(0, this->seq.size()-1) == s) \
        || (s.back() == '*' && s.substr(0, s.size()-1) == this->seq);
}
void Domain::setStrandId(int id) {
    this->strand_id = id;
}
void Domain::setNextId(int id) {
    this->next_id = id;
}
void Domain::setPrevId(int id) {
    this->prev_id = id;
}
int Domain::getNextId() {
    return this->next_id;
}
int Domain::getPrevId() {
    return this->prev_id;
}
int Domain::getStrandId() {
    return this->strand_id;
}
int Domain::getBoundId() {
    return this->bound_id;
}
uint Domain::getRenderLength() {
    if (this->isToehold()) return 30;
    return 40;
}
domainshape Domain::getShape() {
    return this->shape;
}
void Domain::setShape(domainshape sh) {
    this->shape = sh;
}
bool Domain::exists(int id) {
    return Domain::objects.find(id) != Domain::objects.end();
}
void Domain::unbind() {
    if (this->bound_id != -1) {
        DOMAINS[this->bound_id]->bound_id = -1;
        this->bound_id = -1;
    }
}
std::tuple<uint, uint, uint> Domain::getRgbColor() {
    uint r = 0, g = 50, b = 100;
    //color of the domains should depend on the name of the domain
    for(uchar c : this->seq) {
        if (c == '*' || c == '^') continue;
        r = (r + c)%255;
        g = (g + c)*5%255;
        b = (b + c)*7%255;
    }
    return std::make_tuple(r, g, b);
}
void Domain::bind(int id1, int id2) {
    //need to check if the two domains are complementary and are not adjacent
    //adjacent domains cannot be bound
    //the two domains can bind only if they are complementary and straight
    Domain *d1 = DOMAINS[id1], *d2 = DOMAINS[id2];
    if (d1->isComplement(*d2) && d1->getShape() == straight && d1->next_id != d2->id && d2->next_id != d1->id) {
        d1->bound_id = id2;
        d2->bound_id = id1;
        STRANDS[d1->getStrandId()]->findBulgeLoops();
        STRANDS[d1->getStrandId()]->findHairpinLoops();
        STRANDS[d2->getStrandId()]->findBulgeLoops();
        STRANDS[d2->getStrandId()]->findHairpinLoops();
    } else {
        std::cerr << "Error in binding! Domains are not complementary!\n";
    }
}
std::map<int, Domain*>& Domain::getObjects() {
    return Domain::objects;
}
const std::string& Domain::getSeq() {
    return this->seq;
}
const int& Domain::getLength() {
    return this->length;
}
std::string Domain::getInfo() const {
    std::stringstream ss;
    ss << "Domain(id : ";
    ss << id;
    ss << ", seq : ";
    ss << seq;
    ss << ", length : ";
    ss << length;
    ss << ", strand_id : ";
    ss << strand_id;
    ss << ", bound_id : ";
    ss << bound_id;
    ss << ")";
    std::string info = ss.str();
    return info;
}

int Domain::getNewId() {
    return Domain::new_id;
}
