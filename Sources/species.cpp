//
//  species.cpp
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <sstream>
#include "Headers/domain.h"
#include "Headers/strand.h"
#include "Headers/species.h"

using namespace dsd;

std::map<int, Species*> Species::objects;
int Species::new_id = 1;

Species::Species(const std::vector<int>& strands, const std::vector<std::pair<int, int>>& binds, int id) {
    assert(!Species::exists(id));
    //assert that the a species with the given id doesn't exist
    for(int strand_id : strands) {
        this->strands.push_back(strand_id);
        dsd::Strand::getObjects()[strand_id]->setSpeciesId(id);
    }
    for(std::pair<int, int> p : binds) {
        assert(Domain::exists(p.first));
        assert(Domain::exists(p.second));
        Domain::bind(p.first, p.second);
        //assert that a domain with the given id exists
    }
    this->id = id;
    Species::objects[id] = static_cast<Species*>(this);
    new_id++;
    //when creating a new species, one needs to check if the set of strands provided form a connected component
    //this can be done by traversing the graph of the species and checking if all the strands have been visited
}
Species::~Species() {
    //call the destructor of all the strands present in this species
}
std::map<int, Species*>& Species::getObjects() {
    return Species::objects;
}
bool Species::exists(int id) {
    return (Species::objects.find(id) != Species::objects.end());
}
std::string Species::getInfo() const {
    std::stringstream ss;
    ss << "Species(id : ";
    ss << this->id;
    ss << ", strands : [";
    for(int strand_id : this->strands) {
        ss << strand_id;
        ss << " ";
    }
    ss << "])";
    std::string info = ss.str();
    return info;
}
const std::vector<int>& Species::getStrands() const {
    return this->strands;
}
int Species::getNewId() {
    return Species::new_id;
}
