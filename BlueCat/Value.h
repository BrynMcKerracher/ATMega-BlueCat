#ifndef BYTEBAC_VALUE_H_INCLUDED
#define BYTEBAC_VALUE_H_INCLUDED

#include "Object.h"

namespace BlueCat {
    class Object;
  
    class Value {
    public:
        typedef uint8_t byte_t;

        enum Type : uint8_t {
            Bool,
            Null,
            Number,
            Byte,
            Object
        };

        Value();
        explicit Value(bool value);
        explicit Value(double value);
        explicit Value(byte_t value);
        explicit Value(BlueCat::Object* object);

        bool IsType(Value::Type valueType) const;
        bool IsObjectType(Object::Type objectType) const;
        bool IsFalsey() const;

        bool AsBool() const;
        double AsNumber() const;
        byte_t AsByte() const;
        BlueCat::Object* AsObject() const;

        bool operator==(const Value& rhs) const;
    protected:
        friend class Printer;
        friend class Serialiser;
        friend class Deserialiser;

        union ValueUnion {
            bool boolean;
            double number;
            byte_t byte;
            BlueCat::Object* object;
        } as {};

        Type type;
    };
}

#endif //BYTEBAC_VALUE_H_INCLUDED
