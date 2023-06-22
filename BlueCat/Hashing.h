#ifndef BYTEBAC_HASHING_H_INCLUDED
#define BYTEBAC_HASHING_H_INCLUDED

#include "Common.h"

namespace BlueCat {
    class Hashing {
    public:
        static size_t GenerateHash(uint8_t* memstart, size_t memlength);
    };
}

#endif //BYTEBAC_HASHING_H_INCLUDED
