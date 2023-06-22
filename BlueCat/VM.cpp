#include "Common.h"
#include "VM.h"
#include "Debug/Debug.h"
#include "Debug/Printer.h"

//#define BYTEBAC_STACK_TRACE_ENABLED
//#define BYTEBAC_ASM_OUTPUT_ENABLED

#ifdef BYTEBAC_STACK_TRACE_ENABLED
#include <iostream> //For std::cout
#endif //BYTEBAC_STACK_TRACE_ENABLED

BlueCat::VirtualMachine::InterpretResult BlueCat::VirtualMachine::Interpret(const char* source, InterpretMode mode) {
    Segment seg;
    Compiler compiler;
    if (mode != InterpretMode::InterpretOnly && !compiler.Compile(source, seg)) {
        return InterpretResult::CompileError;
    }

    if (mode == InterpretMode::CompileOnly) {
        seg.WriteToFile("test.bcasm");
        return InterpretResult::Ok;
    }
    else if (mode == InterpretMode::InterpretOnly) {
        seg.LoadFromFile("test.bcasm");
    }

    segment = &seg;
    pc = &segment->instructions[0];

    InterpretResult result = Run();

    segment = nullptr;
    return result;
}

BlueCat::VirtualMachine::InterpretResult BlueCat::VirtualMachine::Run() {
    #ifdef BYTEBAC_ASM_OUTPUT_ENABLED
    Debug::DisassembleSegment(*segment);
    #endif //BYTEBAC_ASM_OUTPUT_ENABLED
    while (true) {
        Op instruction;
        #ifdef BYTEBAC_STACK_TRACE_ENABLED
            for (size_t i = 0; stackTop > stack && i < stackTop - stack; ++i) {
                std::cout << "[";
                stack[i].Print();
                std::cout << "]\n";
            }
            std::cout << "\n";
        #endif //BYTEBAC_STACK_TRACE_ENABLED
        switch (instruction = ReadOp()) {
            case Op::Add: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() + b.AsNumber()));
                }
                else if (a.IsObjectType(Object::Type::String) && b.IsObjectType(Object::Type::String)) {
                    Push(Value(ObjectString::Concatenate((BlueCat::ObjectString*)a.AsObject(), (BlueCat::ObjectString*)b.AsObject())));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() + b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Addition operands must either be two numbers, two strings, or two bytes");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseAnd: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() & b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Bitwise-and operands must be two bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseNot: {
                Value a = Pop();
                if (a.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)~a.AsByte()));
                }
                else {
                    CreateRuntimeError("Wrong operand type: The bitwise-not operand must be a byte.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseOr: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() | b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Bitwise-or operands must be two bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseShiftLeft: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() << b.AsByte())));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Number)) {
                    Push(Value((uint8_t)(a.AsByte() << (uint8_t)b.AsNumber())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Bitwise left-shift operands must be a byte, followed by a byte or a number.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseShiftRight: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() >> b.AsByte())));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Number)) {
                    Push(Value((uint8_t)(a.AsByte() >> (uint8_t)b.AsNumber())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Bitwise right-shift operands must be a byte, followed by a byte or a number.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::BitwiseXOR: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() ^ b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Bitwise-XOR operands must be two bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Constant: {
                Push(segment->GetConstant(ReadAddress()));
                break;
            }
            case Op::Decrement: {
                if (Peek(0).IsType(Value::Type::Number)) {
                    Push(Value(Pop().AsNumber() - 1));
                }
                else if (Peek(0).IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(Pop().AsByte() - 1)));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Decrement '--' operator only applies to numbers or bytes");
                    return InterpretResult::RuntimeError;
                }

                break;
            }
            case Op::DefineGlobal: {
                ObjectString* name = ReadString();
                globals.Insert(name, Peek(0));
                Pop();
                break;
            }
            case Op::Divide: {
                Value b = Pop();
                Value a = Pop(); 
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() / b.AsNumber()));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() / b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Division operands must be numbers or bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Equal: {
                Value b = Pop();
                Value a = Pop();
                Push(Value(a == b));
                break;
            }
            case Op::Exponent: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(pow(a.AsNumber(), b.AsNumber())));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value(pow(a.AsByte(), b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Exponent operands must be numbers or bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::False: {
                Push(Value(false));
                break;
            }
            case Op::GetGlobal: {
                ObjectString* name = ReadString();
                Value value;
                if (!globals.Get(name, value)) {
                    CreateRuntimeError("Undefined variable name: ");
                    std::cout << "'" << name->characters << "'\n";
                    return InterpretResult::RuntimeError;
                }
                Push(value);
                break;
            }
            case Op::GetLocal: {
                Push(stack[ReadAddress()]);
                break;
            }
            case Op::Greater: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() > b.AsNumber()));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value(a.AsByte() > b.AsByte()));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Comparison operands must be numbers or bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Increment: {
                if (Peek(0).IsType(Value::Type::Number)) {
                    Push(Value(Pop().AsNumber() + 1));
                }
                else if (Peek(0).IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(Pop().AsByte() + 1)));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Increment '++' operator only applies to numbers or bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Jump: {
                Board::AddressOffset offset = ReadAddressOffset();
                pc += offset;
                break;
            }
            case Op::JumpIfFalse: {
                Board::AddressOffset offset = ReadAddressOffset();
                if (Peek(0).IsFalsey()) {
                    pc += offset;
                }
                break;
            }
            case Op::Less: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() < b.AsNumber()));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value(a.AsByte() < b.AsByte()));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Comparison operands must be numbers or bytes");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Loop: {
                Board::AddressOffset offset = ReadAddressOffset();
                pc -= offset;
                break;
            }
            case Op::Multiply: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() * b.AsNumber()));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() * b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Multiplication operands must be numbers or bytes");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Negate: {
                if (!Peek(0).IsType(Value::Type::Number)) {
                    CreateRuntimeError("Wrong operand type: Negation operand must be a number.");
                    return InterpretResult::RuntimeError;
                }
                Push(Value(-Pop().AsNumber()));
                break;  
            }
            case Op::Nil: {
                Push(Value());
                break;
            }
            case Op::Not: {
                Push(Value(Pop().IsFalsey()));
                break;
            }
            case Op::Pop: {
                PopNoAlloc();
                break;
            }
            case Op::PopN: {
                PopNoAlloc(ReadShortAddress());
                break;
            }
            case Op::Print: {
                //Printer::PrintValue(Pop());
                break;
            }
            case Op::Remainder: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    const double aAsNum = a.AsNumber();
                    const double bAsNum = b.AsNumber();
                    Push(Value(aAsNum - (int)(aAsNum / bAsNum) * bAsNum));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() % b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Remainder operands must be numbers.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::Return: {
                return InterpretResult::Ok;
            }
            case Op::SetGlobal: {
                ObjectString* name = ReadString();
                if (globals.Set(name, Peek(0))) {
                    //If the value doesn't already exist, then error.
                    globals.Remove(name);
                    CreateRuntimeError("Undefined variable set.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::SetLocal: {
                stack[ReadAddress()] = Peek(0);
                break;
            }
            case Op::Subtract: {
                Value b = Pop();
                Value a = Pop();
                if (a.IsType(Value::Type::Number) && b.IsType(Value::Type::Number)) {
                    Push(Value(a.AsNumber() - b.AsNumber()));
                }
                else if (a.IsType(Value::Type::Byte) && b.IsType(Value::Type::Byte)) {
                    Push(Value((uint8_t)(a.AsByte() - b.AsByte())));
                }
                else {
                    CreateRuntimeError("Wrong operand type: Subtraction operands must be two numbers or two bytes.");
                    return InterpretResult::RuntimeError;
                }
                break;
            }
            case Op::True: {
                Push(Value(true));
                break;
            }
        }
    }
}

void BlueCat::VirtualMachine::CreateRuntimeError(const char* error) {
    std::cout << error;
}

BlueCat::Op BlueCat::VirtualMachine::ReadOp() {
    return (*pc++);
}

BlueCat::Board::Address BlueCat::VirtualMachine::ReadAddress() {
    BlueCat::Board::Address address = 0;
    for (uint8_t i = 0; i < Board::OpsPerAddress; ++i) {
        address |= (((BlueCat::Board::Address)ReadOp()) << (8 * sizeof(Op) * i));
    }
    return address;
}

BlueCat::Board::ShortAddress BlueCat::VirtualMachine::ReadShortAddress() {
    BlueCat::Board::ShortAddress address = 0;
    for (uint8_t i = 0; i < Board::OpsPerShortAddress; ++i) {
        address |= (((BlueCat::Board::ShortAddress)ReadOp()) << (8 * sizeof(Op) * i));
    }
    return address;
}

BlueCat::Board::AddressOffset BlueCat::VirtualMachine::ReadAddressOffset() {
    BlueCat::Board::AddressOffset offset = 0;
    for (uint8_t i = 0; i < Board::OpsPerAddressOffset; ++i) {
        offset |= (((BlueCat::Board::AddressOffset)ReadOp()) << (8 * sizeof(Op) * i));
    }
    return offset;
}

BlueCat::ObjectString* BlueCat::VirtualMachine::ReadString() {
    return (ObjectString*)segment->GetConstant(ReadAddress()).AsObject();
}

void BlueCat::VirtualMachine::Push(Value value) {
    stack.Push(value);
}

BlueCat::Value BlueCat::VirtualMachine::Pop() {
    return stack.Pop();
}

void BlueCat::VirtualMachine::PopNoAlloc(size_t n) {
    stack.PopNoAlloc(n);
}

const BlueCat::Value& BlueCat::VirtualMachine::Peek(BlueCat::Board::AddressOffset offset) const {
    return stack.Peek(offset);
}
