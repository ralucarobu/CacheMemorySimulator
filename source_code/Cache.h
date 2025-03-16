#pragma once
#include <string>
#include <sstream>
#include "CacheSet.h"

class Cache {
private:
    std::vector<CacheSet> cacheSets;

public:
    Cache(const std::vector<CacheSet>& cacheSets);
    std::vector<CacheSet>& getCacheSets();

};
