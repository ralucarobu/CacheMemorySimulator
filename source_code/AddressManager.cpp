#include "AddressManager.h"
#include <stdexcept>

AddressManager::AddressManager(int addressWidth, int cacheSize, int blockSize, int associativity)
    : addressWidth(addressWidth), cacheSize(cacheSize), blockSize(blockSize), associativity(associativity) {

    offsetWidth = log2(blockSize);
    indexWidth = log2(cacheSize / (blockSize * associativity));
    tagWidth = addressWidth - indexWidth - offsetWidth;
}

int AddressManager::getNbOfRAMBlocks() const {
    if (blockSize <= 0) {
        throw std::runtime_error("Block size must be greater than 0.");
    }
    return (1 << addressWidth) / blockSize; 
}


int AddressManager::getAddressWidth() const {
    return addressWidth;
}


int AddressManager::getCacheSize() const {
    return cacheSize;
}


int AddressManager::getBlockSize() const {
    return blockSize;
}


int AddressManager::getAssociativity() const {
    return associativity;
}


int AddressManager::getTagWidth() const {
    return tagWidth;
}


int AddressManager::getIndexWidth() const {
    return indexWidth;
}


int AddressManager::getOffsetWidth() const {
    return offsetWidth;
}

int AddressManager::getTag() const {
    return tag;
}

int AddressManager::getIndex() const {
    return index;
}

int AddressManager::getOffset() const {
    return offset;
}

void AddressManager::setTag(int value) {
    tag = value;
}

void AddressManager::setIndex(int value) {
    index = value;
}

void AddressManager::setOffset(int value) {
    offset = value;
}

void AddressManager::getTagIndexOffset_ofAddress(const std::string& binaryAddress) {
    int offsetWidth = getOffsetWidth();
    int indexWidth = getIndexWidth();
    int tagWidth = getTagWidth();

    if (binaryAddress.size() != offsetWidth + indexWidth + tagWidth) {
        throw std::invalid_argument("Invalid binary address length");
    }

 
    if (offsetWidth > 0) {
        offset = std::stoi(binaryAddress.substr(binaryAddress.size() - offsetWidth), nullptr, 2);
    }
    else {
        offset = 0;
    }

    if (indexWidth > 0) {
        index = std::stoi(binaryAddress.substr(binaryAddress.size() - offsetWidth - indexWidth, indexWidth), nullptr, 2);
    }
    else {
        index = 0;
    }

    if (tagWidth > 0) {
        tag = std::stoi(binaryAddress.substr(0, tagWidth), nullptr, 2);
    }
    else {
        tag = 0;
    }
}


int AddressManager::log2(int value) const {
    return static_cast<int>(std::log2(value));
}

