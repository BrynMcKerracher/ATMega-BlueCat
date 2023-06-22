#ifndef BLUECAT_PRINTER_H
#define BLUECAT_PRINTER_H

#include <iostream>
#include "../Value.h"

namespace BlueCat {
    class Printer {
    public:
        static void PrintByte(Value::byte_t byte);
        static void PrintValue(const Value& value);
        static void PrintObject(const Object* obj);
    };
} // BlueCat

#endif //BLUECAT_PRINTER_H
