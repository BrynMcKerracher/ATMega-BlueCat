#ifndef BYTEBAC_LOCAL_H_INCLUDED
#define BYTEBAC_LOCAL_H_INCLUDED

#include "Common.h"
#include "Token.h"

namespace BlueCat {
    class Local {
    public:
        Token name;
        int16_t depth = -1;
    };
}

#endif //BYTEBAC_LOCAL_H_INCLUDED
