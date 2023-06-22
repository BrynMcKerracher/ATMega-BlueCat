#ifndef BYTEBAC_OPCODE_H_INCLUDED
#define BYTEBAC_OPCODE_H_INCLUDED

#include <stdint.h>

namespace BlueCat {
    enum Op : uint8_t {
        Add,
        BitwiseAnd,
        BitwiseNot,
        BitwiseOr,
        BitwiseShiftLeft,
        BitwiseShiftRight,
        BitwiseXOR,
        Constant,
        Decrement,
        DefineGlobal,
        Divide,
        Equal,
        Exponent,
        False,
        GetGlobal,
        GetLocal,
        Greater,
        Increment,
        Jump,
        JumpIfFalse,
        Less,
        Loop,
        Multiply,
        Negate,
        Nil,
        Not,
        Pop,
        PopN,
        Print,
        Remainder,
        Return,
        SetGlobal,
        SetLocal,
        Subtract,
        True
    };
}

#endif //BYTEBAC_OPCODE_H_INCLUDED
