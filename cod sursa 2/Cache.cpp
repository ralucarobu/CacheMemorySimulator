#include "Cache.h"
#include <iomanip>



Cache::Cache(const std::vector<CacheSet>& cacheSets) : cacheSets(cacheSets) {}

std::vector<CacheSet>& Cache::getCacheSets() {
    return cacheSets;
}
