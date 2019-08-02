//
//  species.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <cassert>
#include <map>
#include <vector>
#include "identity.h"

#ifndef SPECIES_H
#define SPECIES_H

#define SPECIES dsd::Species::getObjects()

namespace dsd {
    class Species : public Identity {
    private:
        std::vector<int> strands;
        static std::map<int, Species*> objects;
        static int new_id;
    public:
        Species(const std::vector<int>&, const std::vector<std::pair<int, int>>&, int);
        ~Species();
        std::string getInfo() const;
        const std::vector<int>& getStrands() const;
        static bool exists(int);
        static std::map<int, Species*>& getObjects();
        static int getNewId();
    };
}

#endif // SPECIES_H
