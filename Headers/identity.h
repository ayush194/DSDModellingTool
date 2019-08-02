//
//  identity.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <string>

#ifndef IDENTITY_H
#define IDENTITY_H

namespace dsd {
    //another possible representation of the dna nanomolecules could be a string of text
    //we could overload operators to combine strands to give a text representation of the species
    //but for this we would need a well-defined texual representation of complex species

    //the problem with a textual representation is in recognizing toeholds etc for the purpose of reactions

    //this representation stores strands as list of domains with the 5' to the 3' given from left to right
    //bonds between complementary domains are established by undirected edges between the domains
    //identified by the bound_id property of the domains

    //note that there is a difference between the edges between consecutive domains (which is implicitly
    //established by the next pointer in the list nodes) and the edges created by the bound_id property
    //the difference is that the bound_id property creates H-bonds while the bonds between consecutive
    //domains are actually phosphate links

    class Identity {
        //abstract base class to inherit from
        //all classes which have an id will inherit from this class
    protected:
        int id;
        static int newid;
        //we want to be able to retrieve all instances of a derived class
        std::string info;
        //info retrieves details of the instance as string, can be used for debugging
    public:
        virtual std::string getInfo() const = 0;
        virtual ~Identity();
        //virtual const std::map<int, void*>& getObjects() const = 0;
        int getId() const;
    };
}

#endif // IDENTITY_H
