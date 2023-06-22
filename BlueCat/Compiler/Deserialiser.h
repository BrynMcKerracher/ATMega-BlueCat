#ifndef BLUECAT_DESERIALISER_H
#define BLUECAT_DESERIALISER_H

#include <stdint.h>
#include "../Memory.h"
#include "../Value.h"
#include "../ObjectString.h"

namespace BlueCat {
    class Deserialiser {
    public:
        template <typename T>
        static T HexCharsToByte(char highByte, char lowByte);

        template <typename T>
        static T HexStringToType(const char* str, Board::SizeType offset);

        static uint8_t StreamCharToNybble(unsigned char streamChar);

        static Value HexStringToValue(const char* str);
    };
}

template <typename T>
T BlueCat::Deserialiser::HexCharsToByte(char highByte, char lowByte) {
    T obj;
    uint8_t* p = (uint8_t*)&obj;
    for (size_t i = 0; i < sizeof(T); ++i) {
        uint8_t highNybble = StreamCharToNybble(highByte) << 4;
        uint8_t lowNybble = StreamCharToNybble(lowByte);
        p[i] = highNybble | lowNybble;
    }
    return obj;
}

template <typename T>
T BlueCat::Deserialiser::HexStringToType(const char* str, Board::SizeType offset) {
    T obj;
    uint8_t* p = (uint8_t*)&obj;
    for (size_t i = 0; i < sizeof(T); ++i) {
        uint8_t highNybble = StreamCharToNybble(str[2 * i + offset]) << 4;
        uint8_t lowNybble = StreamCharToNybble(str[2 * i + 1 + offset]);
        p[i] = highNybble | lowNybble;
    }
    return obj;
}

#endif //BLUECAT_DESERIALISER_H
