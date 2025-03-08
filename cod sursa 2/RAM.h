#pragma once

#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>

class RAM {
private:
    std::vector<unsigned char> memory; 
    int addressWidth;                 
    int size;                          

public:
    RAM(int addressWidth);

    void generate();

    std::string getMemoryAsString();

    int getSize() const;

    unsigned char read(int address) const;

    void write(int address, unsigned char value);

    std::vector<unsigned char> getMemory() const;

    std::vector<unsigned char>& getMemoryRef();

};
