#ifndef BYTEBAC_PARSER_H_INCLUDED
#define BYTEBAC_PARSER_H_INCLUDED

#include "Token.h"

namespace BlueCat {
    struct Parser {
        Token current;
        Token previous;
        bool hadError = false;
        bool panicMode = false;
    };
}

#endif //BYTEBAC_PARSER_H_INCLUDED
