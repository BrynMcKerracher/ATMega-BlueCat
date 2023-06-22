#include "Value.h"

BlueCat::Value::Value() {
    this->type = Value::Type::Null;
    as.object = nullptr;
}

BlueCat::Value::Value(bool value) {
    this->type = Value::Type::Bool;
    as.boolean = value;
}

BlueCat::Value::Value(double value) {
    this->type = Value::Type::Number;
    as.number = value;
}

BlueCat::Value::Value(byte_t value) {
    this->type = Value::Type::Byte;
    as.byte = value;
}

BlueCat::Value::Value(BlueCat::Object* value) {
    this->type = Value::Type::Object;
    as.object = value;
}

bool BlueCat::Value::IsType(Value::Type valueType) const {
    return this->type == valueType;
}

bool BlueCat::Value::IsObjectType(Object::Type objectType) const {
    if (this->type != Value::Type::Object) {
        return false;
    }
    return this->as.object->type == objectType;
}

bool BlueCat::Value::IsFalsey() const {
    return type == Type::Null || (type == Type::Bool && !AsBool());
}

bool BlueCat::Value::AsBool() const {
    return as.boolean;
}

double BlueCat::Value::AsNumber() const {
    return as.number;
}

BlueCat::Value::byte_t BlueCat::Value::AsByte() const {
    return as.byte;
}

BlueCat::Object* BlueCat::Value::AsObject() const {
    return as.object;
}

bool BlueCat::Value::operator==(const Value& rhs) const {
    if (this->type != rhs.type) return false;
    switch (this->type) {
        case Type::Bool:    return this->AsBool() == rhs.AsBool();
        case Type::Null:    return true;
        case Type::Number:  return this->AsNumber() == rhs.AsNumber();
        case Type::Byte:    return this->AsByte() == rhs.AsByte();
        case Type::Object:  return this->AsObject() == rhs.AsObject();
        default: return false; //Unreachable
    }
}
