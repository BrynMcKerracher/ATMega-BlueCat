//
// Created by Bryn McKerracher on 19/06/2023.
//

#include "Serialiser.h"
#include "../ObjectString.h"

unsigned char BlueCat::Serialiser::NybbleToStreamChar(uint8_t nybble) {
    return (unsigned char)(nybble >= 10 ? nybble + 'a' - 10 : nybble + '0');
}

std::string BlueCat::Serialiser::ValueToHexString(const Value& value) {
    std::ostringstream stream;
    stream << BytesToHexString(value.type);
    switch (value.type) {
        case Value::Type::Bool: {
            stream << BytesToHexString(value.as.boolean);
            break;
        }
        case Value::Type::Null: {
            break;
        }
        case Value::Type::Number: {
            stream << BytesToHexString(value.as.number);
            break;
        }
        case Value::Type::Byte: {
            stream << BytesToHexString(value.as.byte);
            break;
        }
        case Value::Type::Object: {
            stream << SerialiseObject(value.as.object);
            break;
        }
    }
    stream << "\n";
    return stream.str();
}

std::string BlueCat::Serialiser::SerialiseObject(const Object *obj) {
    switch (obj->type) {
        case Object::String: {
            ObjectString* objString = (ObjectString*)obj;
            std::string str = Serialiser::BytesToHexString(objString->type);
            for (size_t i = 0; i < objString->length; ++i) {
                str += Serialiser::BytesToHexString(objString->characters[i]);
            }
            return str;
        }
    }
    return ""; //Unreachable
}
