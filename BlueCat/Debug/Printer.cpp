#include "Printer.h"
#include "../ObjectString.h"

namespace BlueCat {
    void Printer::PrintByte(Value::byte_t byte) {
        std::cout << "0b";
        for (uint8_t i = 0; i < 8 * sizeof (Value::byte_t); ++i) {
            std::cout << (byte & (1 << (8 * sizeof (Value::byte_t) - 1 - i)) ? '1' : '0');
        }
        std::cout << " (" << (uint16_t)byte << ")";
    }

    void Printer::PrintValue(const Value &value) {
        switch (value.type) {
            case Value::Type::Bool:    std::cout << (value.AsBool() ? "true" : "false"); break;
            case Value::Type::Null:    std::cout << "null";                              break;
            case Value::Type::Number:  std::cout << value.AsNumber();                    break;
            case Value::Type::Byte:    Printer::PrintByte(value.AsByte());          break;
            case Value::Type::Object:  Printer::PrintObject(value.AsObject());       break;
        }
    }

    void Printer::PrintObject(const Object *obj) {
        switch (obj->type) {
            case Object::String: {
                ObjectString* objString = (ObjectString*)obj;
                std::cout << objString->characters;
                break;
            }
        }
    }
} // BlueCat