#include "ReplacementPolicy.h"
#include "CacheLine.h"
#include <stdexcept>
#include <vector>
#include <cstdlib> // for rand 
#include <ctime>   // for time()

void ReplacementPolicy::setPolicy(int policyType) {
    policy = policyType;
}

int ReplacementPolicy::getPolicy() const {
    return policy;
}

int ReplacementPolicy::replaceLineIndex(std::vector<CacheLine>& cacheLines) {
    if (cacheLines.empty()) {
        throw std::runtime_error("No cache lines to replace");
    }

    if (policy == 0) {
        // LRU
        
        int minIndex = 0;
        int minVal = cacheLines[0].getLeastRecentlyUsed();
        for (int i = 1; i < (int)cacheLines.size(); i++) {
            if (cacheLines[i].getLeastRecentlyUsed() < minVal) {
                minVal = cacheLines[i].getLeastRecentlyUsed();
                minIndex = i;
            }
        }
        return minIndex;
    }
    else if (policy == 1) {
        // LFU

        int minIndex = 0;
        int minCount = cacheLines[0].getAccessedLineCount();
        for (int i = 1; i < (int)cacheLines.size(); i++) {
            if (cacheLines[i].getAccessedLineCount() < minCount) {
                minCount = cacheLines[i].getAccessedLineCount();
                minIndex = i;
            }
        }
        return minIndex;
    }
    else if (policy == 2) {
        // Random
        int randIndex = rand() % cacheLines.size();
        return randIndex;
    }

  
    return 0;
}
