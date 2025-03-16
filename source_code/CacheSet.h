#pragma once
#include <vector>
#include "CacheLine.h"

class CacheSet {
private:
    std::vector<CacheLine> cacheLines;

public:
    CacheSet(int associativity, int blockSize);

    std::vector<CacheLine>& getCacheLines();
    const std::vector<CacheLine>& getCacheLines() const; 
};
