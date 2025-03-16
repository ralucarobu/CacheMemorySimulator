#pragma once
#include <cmath> 
#include <string>
class AddressManager {
private:
    int addressWidth;
    int cacheSize;
    int blockSize;
    int associativity;

    int tagWidth;
    int indexWidth;
    int offsetWidth;

    int tag;
    int index;
    int offset;

public:
    AddressManager(int addressWidth, int cacheSize, int blockSize, int associativity);

    int getNbOfRAMBlocks() const;

    int getAddressWidth() const;
    int getCacheSize() const;
    int getBlockSize() const;
    int getAssociativity() const;

    int getTagWidth() const;
    int getIndexWidth() const;
    int getOffsetWidth() const;

    int getTag() const;
    int getIndex() const;
    int getOffset() const;

    void setTag(int value);
    void setIndex(int value);
    void setOffset(int value);
    
    void getTagIndexOffset_ofAddress(const std::string& binaryAddress);

private:
    int log2(int value) const;
};
