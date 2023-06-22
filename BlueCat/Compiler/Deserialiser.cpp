//
// Created by Bryn McKerracher on 19/06/2023.
//

#include "Deserialiser.h"
#include "../Stack.h"

uint8_t BlueCat::Deserialiser::StreamCharToNybble(unsigned char streamChar) {
    return (uint8_t)((streamChar <= '9') ? streamChar - '0' : streamChar - 'a' + 10);
}

BlueCat::Value BlueCat::Deserialiser::HexStringToValue(const char *str) {
    const BlueCat::Board::SizeType TypeHighByte = 0;
    const BlueCat::Board::SizeType TypeLowByte = TypeHighByte + 1;
    const BlueCat::Board::SizeType PrimitiveDataStartHighByte = 2;
    const BlueCat::Board::SizeType PrimitiveDataStartLowByte = PrimitiveDataStartHighByte + 1;
    const BlueCat::Board::SizeType StringObjectDataStartHighByte = 4;
    const BlueCat::Board::SizeType StringSize = strlen(str);
    const uint8_t CharsPerByte = 2;

    Value value;

    value.type = Deserialiser::HexCharsToByte<Value::Type>(str[TypeHighByte], str[TypeLowByte]);
    switch (value.type) {
        case Value::Type::Bool: {
            value.type = Value::Type::Bool;
            value.as.boolean = Deserialiser::HexStringToType<bool>(str, PrimitiveDataStartHighByte);
            break;
        }
        case Value::Type::Null: {
            value.type = Value::Type::Null;
            break;
        }
        case Value::Type::Number: {
            value.type = Value::Type::Number;
            value.as.number = Deserialiser::HexStringToType<double>(str, PrimitiveDataStartHighByte);
            break;
        }
        case Value::Type::Byte: {
            value.type = Value::Type::Byte;
            value.as.byte = Deserialiser::HexStringToType<Value::byte_t>(str, PrimitiveDataStartHighByte);
            break;
        }
        case Value::Type::Object: {
            value.type = Value::Type::Object;
            Object::Type objType = Deserialiser::HexCharsToByte<Object::Type>(str[PrimitiveDataStartHighByte], str[PrimitiveDataStartLowByte]);
            switch (objType) {
                case Object::String: {
                    //Concat string
                    Stack<char> charStream;
                    for (Board::SizeType i = StringObjectDataStartHighByte; i < StringSize; i += CharsPerByte) {
                        charStream.Push(Deserialiser::HexCharsToByte<char>(str[i], str[i + 1]));
                    }
                    value.as.object = ObjectString::CreateTranslatedCopyOfCStr(charStream.GetArray(), charStream.Count());
                    break;
                }
            }
            break;
        }
    }
    return value;
}
