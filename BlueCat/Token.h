#ifndef BYTEBAC_TOKEN_H_INCLUDED
#define BYTEBAC_TOKEN_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

namespace BlueCat {
    class Token {
    public:
        enum Type : uint8_t {
            //Single char
            LeftParen = 0, RightParen,
            LeftBrace, RightBrace,
            Comma, Dot, Minus, Plus,
            Semicolon, Slash, Star, Remainder,
            BitwiseNot, BitwiseOr, BitwiseAnd, BitwiseXOR,
            //One or two char
            Bang, BangEqual,
            Equal, EqualEqual,
            Greater, GreaterEqual,
            Less, LessEqual,
            //Two char
            Increment, Decrement, RawString, Exponent,
            BitwiseShiftLeft, BitwiseShiftRight,
            //Literals
            Identifier, String, Number, Byte,
            //Keywords
            And, Class, Else, False,
            For, Fun, If, Null, Or,
            Print, Return, Super, This,
            True, Var, While,
            //Error
            Error, EndOfFile, 
            //Count (must be at end)
            NumberOfTokens
        };

        Type type;
        const char* start;
        size_t length;
        size_t line;
    };
}

#endif //BYTEBAC_TOKEN_H_INCLUDED
