#ifndef BYTEBAC_HASHABLE_H_INCLUDED
#define BYTEBAC_HASHABLE_H_INCLUDED

#include "Common.h"

namespace BlueCat {
    class Hashable {
    public:
        virtual size_t Hash() = 0;  
    };
}

#endif //#ifndef BYTEBAC_HASHABLE_H_INCLUDED