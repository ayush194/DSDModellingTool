//
//  strand.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <cassert>
#include <vector>
#include <map>
#include "identity.h"

#ifndef STRAND_H
#define STRAND_H

#define STRANDS dsd::Strand::getObjects()
typedef unsigned int uint;

namespace dsd {
    class Strand : public Identity {
    private:
        //the 3' end of the strand is always at the end of the list
        std::vector<int> domains;
        int species_id;
        static std::map<int, Strand*> objects;
        static int new_id;
        //once the strand is created with the specified domains, it cannot be changed
        //stores the ids of the domains
    public:
        Strand(const std::vector<std::string>&, const std::vector<int>&, const std::vector<int>&, int, int=-1);
        ~Strand();
        std::string getInfo() const;
        void setSpeciesId(int);
        int getSpeciesId();
        void findHairpinLoops();
        void findBulgeLoops();
        void findInternalLoops();
        const std::vector<int>& getDomains() const;
        static uint numberOfStrandsInConnectedComponent(int);
        static bool exists(int);
        static std::map<int, Strand*>& getObjects();
        static int getNewId();
        //addDomains(dsd::Domain& ds, ...);
        //addDomains(dsd::Domain ds, ...);
    };
}
#endif // STRAND_H
