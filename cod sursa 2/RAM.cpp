#include "RAM.h"

RAM::RAM(int addressWidth) {
    this->addressWidth = addressWidth;
    this->size = 1 << addressWidth; 
    memory.resize(size);           
}

void RAM::generate() {
    std::random_device rd;                            
    std::mt19937 gen(rd());                          
    std::uniform_int_distribution<> distrib(0, 255);

    for (int i = 0; i < size; ++i) {
        memory[i] = static_cast<unsigned char>(distrib(gen));
    }
}

std::string RAM::getMemoryAsString() {
    std::ostringstream oss;
    for (int i = 0; i < size; i += 8) {
        oss << "0x" << std::hex << std::setw(2) << std::setfill('0') << i << ": ";
        for (int j = 0; j < 8 && i + j < size; ++j) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(memory[i + j]) << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

int RAM::getSize() const {
    return size;
}

unsigned char RAM::read(int address) const {
    if (address < 0 || address >= size) {
        throw std::out_of_range("Address out of range");
    }
    return memory[address];
}

void RAM::write(int address, unsigned char value) {
    if (address < 0 || address >= size) {
        throw std::out_of_range("Address out of range");
    }
    memory[address] = value;
}

std::vector<unsigned char> RAM::getMemory() const {
    return memory;
}
std::vector<unsigned char>& RAM::getMemoryRef() {
    return memory;
}


