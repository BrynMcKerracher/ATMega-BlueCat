#ifndef BLUECAT_OBJECTSTRING_H_INCLUDED
#define BLUECAT_OBJECTSTRING_H_INCLUDED

#include "Common.h"
#include "Object.h"
#include "Memory.h"

namespace BlueCat {
    class ObjectString : public Object {
    public:        
        ~ObjectString() noexcept override;

        static ObjectString* Concatenate(ObjectString* a, ObjectString* b);
        static ObjectString* CreateFromOwnershipOfCStr(char* string, Board::SizeType strLength);
        static ObjectString* CreateFromCopyOfCStr(const char* string, Board::SizeType strLength);
        static ObjectString* CreateTranslatedCopyOfCStr(const char* string, Board::SizeType strLength);

        bool operator==(const Object& rhs) const override;

        Board::SizeType length = 0;
        char* characters = nullptr;
    protected:
        ObjectString(char* string, Board::SizeType strLength);
    };
}

#endif //BLUECAT_OBJECTSTRING_H_INCLUDED
