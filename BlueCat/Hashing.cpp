#include "Hashing.h"

size_t BlueCat::Hashing::GenerateHash(uint8_t* memstart, size_t memlength) {
    //FNV-1a Hash (for 32-bit hashing)
    uint32_t keyHash = 2166136261u;
    for (size_t i = 0; i < memlength; ++i) {
        keyHash ^= (uint8_t)*(memstart + i);
        keyHash *= 16777619;
    }

    if (sizeof(size_t) == 2) { //If in a 16-bit address space, XOR-fold the address to fit.
        keyHash ^= (uint16_t)(keyHash >> 16); //Fold high bits into low bits, in-place.
    } 
    return keyHash;
}
