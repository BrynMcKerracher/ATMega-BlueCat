#ifndef BYTEBAC_OBJECT_H_INCLUDED
#define BYTEBAC_OBJECT_H_INCLUDED

#include "Common.h"

namespace BlueCat {
    struct Object {
        enum Type : uint8_t {
            String
        };

        virtual bool operator==(const Object& rhs) const = 0;
        virtual ~Object() noexcept = default;

        Type type = String;
    };
}

#endif //BYTEBAC_OBJECT_H_INCLUDED
