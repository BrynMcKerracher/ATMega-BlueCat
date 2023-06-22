#ifndef BLUECAT_SERIALISER_H
#define BLUECAT_SERIALISER_H

#include <sstream>
#include "../Value.h"

namespace BlueCat {
    class Serialiser {
    public:
        template<typename T>
        static std::string BytesToHexString(const T &data);

        static unsigned char NybbleToStreamChar(uint8_t nybble);
        static std::string ValueToHexString(const Value &value);
        static std::string SerialiseObject(const Object* obj);
    };
}

template <typename T>
std::string BlueCat::Serialiser::BytesToHexString(const T &data) {
    std::ostringstream stream;
    uint8_t* p = (uint8_t*)&data;
    for (size_t i = 0; i < sizeof(T); ++i) {
        const uint8_t highNybble = p[i] >> 4;
        const uint8_t lowNybble = p[i] & 0x0f;

        stream << NybbleToStreamChar(highNybble);
        stream << NybbleToStreamChar(lowNybble);
    }
    return stream.str();
}

#endif //BLUECAT_SERIALISER_H
