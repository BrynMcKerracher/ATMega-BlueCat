#ifndef BYTEBAC_VM_H_INCLUDED
#define BYTEBAC_VM_H_INCLUDED

#include "Segment.h"
#include "OpCode.h"
#include "Compiler/Compiler.h"
#include "HashTable.h"
#include "Stack.h"

namespace BlueCat {
    class VirtualMachine {
    public:
        enum InterpretResult {
            Ok,
            CompileError,
            RuntimeError  
        };

        enum InterpretMode {
            CompileAndInterpret,
            CompileOnly,
            InterpretOnly
        };

        InterpretResult Interpret(const char* source, InterpretMode mode = InterpretMode::CompileAndInterpret);
    protected:
        Segment* segment = nullptr;
        Op* pc = nullptr;
        Stack<Value> stack;
        HashTable<ObjectString*, Value> globals;

        InterpretResult Run();
        static void CreateRuntimeError(const char* error);
        Op ReadOp();
        Board::Address ReadAddress();
        Board::ShortAddress ReadShortAddress();
        Board::AddressOffset ReadAddressOffset();
        ObjectString* ReadString();
        void Push(Value value);
        Value Pop();
        void PopNoAlloc(size_t n = 1);
        const Value& Peek(Board::AddressOffset offset) const;
    };
}

#endif //BYTEBAC_VM_H_INCLUDED
