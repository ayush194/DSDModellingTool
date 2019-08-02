#ifndef DSDPARSER_H
#define DSDPARSER_H

#include <string>
#include <vector>
#include <Headers/domain.h>
#include <Headers/strand.h>
#include <Headers/species.h>

/*
 * Parses a text file containing the species in the BNGL format
 * Identifies the strands and species in the file and creates
 * their respective objects in memory
 */

class DSDParser {
public:
    DSDParser();
    ~DSDParser();
    void parseInput(std::string, std::string="//");
    void parseInputSpecies(std::vector<std::string>&);
    void parseSpecies(std::string, std::string="//");
};

#endif // DSDPARSER_H
