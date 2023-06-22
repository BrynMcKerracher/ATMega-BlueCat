#ifndef BYTEBAC_MEMORY_H_INCLUDED
#define BYTEBAC_MEMORY_H_INCLUDED

#include <stdlib.h>
#include "OpCode.h"

namespace BlueCat::Board {
    extern const size_t RAM;
    extern const char* Name;

    typedef uint16_t Address;
    typedef uint8_t ShortAddress;
    typedef int16_t AddressOffset;
    typedef uint16_t SizeType;
    static constexpr uint8_t OpsPerAddress = sizeof(Address) / sizeof(Op);
    static constexpr uint8_t OpsPerShortAddress = sizeof(ShortAddress) / sizeof(Op);
    static constexpr uint8_t OpsPerAddressOffset = sizeof(AddressOffset) / sizeof(Op);
}

#endif //BYTEBAC_MEMORY_H_INCLUDED
