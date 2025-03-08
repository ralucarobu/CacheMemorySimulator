#pragma once
#ifndef REPLACEMENTPOLICY_H
#define REPLACEMENTPOLICY_H

#include <vector>

class CacheLine;

class ReplacementPolicy {
private:
    int policy; // 0 = LRU, 1 = LFU, 2 = Random

public:
    ReplacementPolicy() : policy(0) {}

    void setPolicy(int policyType);
    int getPolicy() const;

 
    int replaceLineIndex(std::vector<CacheLine>& cacheLines);
};

#endif
