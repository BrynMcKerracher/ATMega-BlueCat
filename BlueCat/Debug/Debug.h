#ifndef BYTEBAC_DEBUG_H_INCLUDED
#define BYTEBAC_DEBUG_H_INCLUDED

#include "../Segment.h"
#include "../Value.h"

namespace BlueCat {
    class Debug {
        public:
            static void DisassembleSegment(Segment& segment);
            static size_t DisassembleInstruction(Segment& segment, size_t offset);
        protected:
            static size_t SimpleInstruction(const char* name, size_t offset);
            static size_t ShortInstruction(const char* name, Segment& segment, size_t offset);
            static size_t ConstantInstruction(const char* name, Segment& segment, size_t offset);
            static size_t AddressInstruction(const char* name, Segment& segment, size_t offset);
            static size_t JumpInstruction(const char* name, int8_t sign, Segment& segment, size_t offset);
    };
}

#endif //BYTEBAC_DEBUG_H_INCLUDED
