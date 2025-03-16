#include "CacheLine.h"
#include <stdexcept>

CacheLine::CacheLine(int blockSize)
    : validBit(false), dirtyBit(false), tag(-1), cacheLineData(blockSize, 0), accessedLineCount(0) {}

bool CacheLine::isValid() const {
    return validBit;
}

void CacheLine::setValid(bool valid) {
    validBit = valid;
}

bool CacheLine::isDirty() const {
    return dirtyBit;
}

void CacheLine::setDirty(bool dirty) {
    dirtyBit = dirty;
}

int CacheLine::getTag() const {
    return tag;
}

void CacheLine::setTag(int newTag) {
    tag = newTag;
}

std::vector<unsigned char>& CacheLine::getCacheLineData() {
    return cacheLineData;
}

void CacheLine::setCacheLineData(const std::vector<unsigned char>& data) {
    if (data.size() != cacheLineData.size()) {
        throw std::invalid_argument("Data size does not match block size.");
    }
    cacheLineData = data;
}

void CacheLine::updateLine(const std::vector<unsigned char>& data) {
    if (data.size() != cacheLineData.size()) {
        throw std::invalid_argument("Data size does not match block size.");
    }
    cacheLineData = data;
    validBit = true;
    
}

void CacheLine::initializeLine() {
    validBit = false;
    dirtyBit = false;
    accessedLineCount = 0; 
    this->least_recently_used = 0;
}

int CacheLine::getAccessedLineCount() const {
    return accessedLineCount;
}

void CacheLine::setAccessedLineCount(int count) {
    accessedLineCount = count;
}

void CacheLine::setLeastRecentlyused(int lru) {
    this->least_recently_used = lru;
}

int CacheLine::getLeastRecentlyUsed()
{
    return this->least_recently_used;
}



