#pragma once
#include <vector>


class CacheLine {
private:
    bool validBit;
    bool dirtyBit;
    int tag;
    std::vector<unsigned char> cacheLineData;
   
    int accessedLineCount;
    int least_recently_used;

public:
    CacheLine(int blockSize);
    bool isValid() const;
    void setValid(bool valid);

    bool isDirty() const;
    void setDirty(bool dirty);

    int getTag() const;
    void setTag(int tag);

    std::vector<unsigned char>& getCacheLineData();
    void setCacheLineData(const std::vector<unsigned char>& data);
    void updateLine(const std::vector<unsigned char>& data);

    void initializeLine();

    int getAccessedLineCount() const;
    void setAccessedLineCount(int count);
    void incrementAccessedLineCount();

    void setLeastRecentlyused(int lru);
    int getLeastRecentlyUsed();
};
