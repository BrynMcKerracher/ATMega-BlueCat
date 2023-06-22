#ifndef BYTEBAC_SEGMENT_H_INCLUDED
#define BYTEBAC_SEGMENT_H_INCLUDED

#include <stdint.h>

#include "Stack.h"
#include "OpCode.h"
#include "Value.h"
#include "Memory.h"

namespace BlueCat {
    class Segment {
        public:
            friend class Debug;
            friend class VirtualMachine;
            friend class Compiler;
            
            Board::SizeType AddConstant(const Value& value);
            Value GetConstant(Board::SizeType index);
            void AddOp(Op op);
            Board::Address ReadAddress(Board::SizeType index);
            Board::ShortAddress ReadShortAddress(Board::SizeType index);
            bool WriteToFile(const char* name) const;
            bool LoadFromFile(const char* name);
            Board::SizeType InstructionCount() const;
        protected:
            Stack<Op> instructions;
            Stack<Value> constants;
    };
}

#endif //BYTEBAC_SEGMENT_H_INCLUDED
