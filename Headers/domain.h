//
//  domain.h
//  DSDModellingTool
//
//  Created by Ayush Kumar on 20/5/19.
//

#include <cassert>
#include <map>
#include "identity.h"

#ifndef DOMAIN_H
#define DOMAIN_H

#define DOMAINS dsd::Domain::getObjects()

typedef unsigned int uint;
typedef unsigned char uchar;

namespace dsd {
    enum domainshape {hairpin, bulge, internal, branch, straight};
    class Domain : public Identity {
    private:
        const std::string seq;
        //any domain of length <= 9 will be a toehold domain
        //and any domain of length >9 is a recognition domain
        static std::map<int, Domain*> objects;
        static int new_id;
        //'*' at the end of seq to identify a complementary domain,
        //the remaining string should only contain alphanumeric charaters [a-zA-Z0-9]
        int strand_id;
        int bound_id;
        int length;
        int next_id;
        int prev_id;
        domainshape shape;
        //bound points to the the other bound domain if it is, else it is -1
        //strand_id is the id of the strand of which this is a part of
        //next_id and prev_id points to the next and previous domains in the strand
        //next_id/prev_domain is -1 if it is the last/first domain in the strand
    public:
        Domain(const std::string&, int, int, int);
        ~Domain();
        bool isToehold() const;
        bool isComplement(const Domain&) const;
        bool isComplement(const std::string&) const;
        void setStrandId(int);
        void setNextId(int);
        void setPrevId(int);
        int getNextId();
        int getPrevId();
        int getStrandId();
        int getBoundId();
        void setShape(domainshape);
        domainshape getShape();
        uint getRenderLength();
        std::tuple<uint, uint, uint> getRgbColor();
        void unbind();
        static void bind(int, int);
        const std::string& getSeq();
        const int& getLength();
        std::string getInfo() const;
        static bool exists(int);
        static std::map<int, Domain*>& getObjects();
        static int getNewId();
    };
}

#endif // DOMAIN_H
