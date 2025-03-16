#include "CacheSet.h"


CacheSet::CacheSet(int associativity, int blockSize) {
    for (int i = 0; i < associativity; ++i) {
        CacheLine line(blockSize);
        line.initializeLine(); 
        cacheLines.push_back(line);
    }
}

std::vector<CacheLine>& CacheSet::getCacheLines() {
    return cacheLines;
}

const std::vector<CacheLine>& CacheSet::getCacheLines() const {
    return cacheLines;
}

