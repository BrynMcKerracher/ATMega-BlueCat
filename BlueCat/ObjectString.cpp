#include "ObjectString.h"

BlueCat::ObjectString::ObjectString(char* string, Board::SizeType strLength) {
    type = Object::Type::String;
    length = strLength;
    characters = string;
}

BlueCat::ObjectString::~ObjectString() noexcept {
    delete [] characters;
}

BlueCat::ObjectString* BlueCat::ObjectString::Concatenate(BlueCat::ObjectString* a, BlueCat::ObjectString* b) {
    const size_t stringSize = a->length + b->length;
    char* combinedString = new char[stringSize + 1];
    strncpy(combinedString, a->characters, a->length);
    strncpy(combinedString + a->length, b->characters, b->length);
    combinedString[stringSize] = '\0';
    return new BlueCat::ObjectString(combinedString, stringSize);
}

BlueCat::ObjectString* BlueCat::ObjectString::CreateFromOwnershipOfCStr(char* string, Board::SizeType strLength) {
    return new ObjectString(string, strLength);
}

BlueCat::ObjectString* BlueCat::ObjectString::CreateFromCopyOfCStr(const char* string, Board::SizeType strLength) {
    char* stringCopy = new char[strLength + 1];
    strncpy(stringCopy, string, strLength);
    stringCopy[strLength] = '\0';
    return new ObjectString(stringCopy, strLength);
}

BlueCat::ObjectString* BlueCat::ObjectString::CreateTranslatedCopyOfCStr(const char* string, Board::SizeType strLength) {
    Board::SizeType escapedBufferSize = strLength;

    //Find num escaped chars
    for (Board::SizeType i = 0; i < strLength; ++i) {
        if (string[i] == '\\') {
            switch (string[i + 1]) {
                case 'n':
                case 't':
                case 'r':
                case 'b':;
                case 'v':
                    escapedBufferSize--; //Each escape char reduces new size by one because we condense 2 chars into 1 escaped one.
                    break;
                default: break;
            }
        }
    }

    char* stringCopy = new char[escapedBufferSize + 1];
    Board::SizeType copyOffset = 0;
    //Translate chars
    for (Board::SizeType i = 0; i < strLength; ++i) {
        if (string[i] == '\\') {
            switch (string[i + 1]) {
                case 'n': {
                    stringCopy[i - copyOffset] = '\n';
                    ++copyOffset;
                    ++i;
                    break;
                }
                case 't': {
                    stringCopy[i - copyOffset] = '\t';
                    ++copyOffset;
                    ++i;
                    break;
                }
                case 'r': {
                    stringCopy[i - copyOffset] = '\r';
                    ++copyOffset;
                    ++i;
                    break;
                }
                case 'b': {
                    stringCopy[i - copyOffset] = '\b';
                    ++copyOffset;
                    ++i;
                    break;
                }
                case 'v': {
                    stringCopy[i - copyOffset] = '\v';
                    ++copyOffset;
                    ++i;
                    break;
                }
                default: break; //Parse as a normal character if not escaping
            }
        }
        else {
            stringCopy[i - copyOffset] = string[i];
        }
    }
    stringCopy[escapedBufferSize] = '\0';
    return new ObjectString(stringCopy, escapedBufferSize);
}

bool BlueCat::ObjectString::operator==(const Object& rhs) const {
    if (rhs.type != Object::Type::String) return false;
    ObjectString* rhsString = (ObjectString*)&rhs;
    return this->length == rhsString->length && memcmp(this->characters, rhsString->characters, length) == 0;
}
