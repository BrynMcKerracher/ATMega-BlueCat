#include "../Common.h"
#include "Debug.h"
#include "Printer.h"
#include <iostream>
#include <iomanip>

void BlueCat::Debug::DisassembleSegment(Segment& segment) {
    for (size_t offset = 0; offset < segment.instructions.Count();) {
        offset = DisassembleInstruction(segment, offset);
    }
    std::cout << "=======================================================================\n";
}

size_t BlueCat::Debug::SimpleInstruction(const char* name, size_t offset) {
    std::cout << std::setw(25) << name << "\n";
    return offset + 1;
}

size_t BlueCat::Debug::ShortInstruction(const char *name, BlueCat::Segment &segment, size_t offset) {
    BlueCat::Board::ShortAddress address = segment.ReadShortAddress(offset + 1);
    std::cout << std::setw(25) << name << std::setw(10) << " Short Address: " << (unsigned)address << "\n";
    return offset + Board::OpsPerShortAddress + 1;
}

size_t BlueCat::Debug::ConstantInstruction(const char* name, Segment& segment, size_t offset) {
    BlueCat::Board::Address constantAddress = segment.ReadAddress(offset + 1);
    std::cout << std::setw(25) << std::left << name << std::setw(10) << " Constant: (" << constantAddress << ") '";
    Printer::PrintValue(segment.constants[constantAddress]);
    std::cout << "'\n";
    return offset + Board::OpsPerAddress + 1;
}

size_t BlueCat::Debug::AddressInstruction(const char* name, Segment& segment, size_t offset) {
    BlueCat::Board::Address localIndex = segment.ReadAddress(offset + 1);
    std::cout << std::setw(25) << name << std::setw(10) << " Local: (" << localIndex << ")\n";
    return offset + Board::OpsPerAddress + 1;
}

size_t BlueCat::Debug::JumpInstruction(const char* name, int8_t sign, Segment& segment, size_t offset) {
    BlueCat::Board::Address jump = segment.ReadAddress(offset + 1);
    std::cout << std::setw(25) << name << std::setw(10) << " Jump: " << offset << " -> " << (offset + sign * jump + 3) << "\n";
    return offset + 3;
}

size_t BlueCat::Debug::DisassembleInstruction(Segment& segment, size_t offset) {
    std::cout << offset << "\t";

    Op op = segment.instructions[offset];
    switch (op) {
        case Op::Add:
            return SimpleInstruction("OP_ADD", offset);
        case Op::BitwiseAnd:
            return SimpleInstruction("OP_BITWISE_AND", offset);
        case Op::BitwiseNot:
            return SimpleInstruction("OP_BITWISE_NOT", offset);
        case Op::BitwiseOr:
            return SimpleInstruction("OP_BITWISE_OR", offset);
        case Op::BitwiseShiftLeft:
            return SimpleInstruction("OP_BITWISE_SHIFT_LEFT", offset);
        case Op::BitwiseShiftRight:
            return SimpleInstruction("OP_BITWISE_SHIFT_RIGHT", offset);
        case Op::BitwiseXOR:
            return SimpleInstruction("OP_BITWISE_XOR", offset);
        case Op::Constant:
            return ConstantInstruction("OP_CONSTANT", segment, offset);
        case Op::Decrement:
            return SimpleInstruction("OP_DECREMENT", offset);
        case Op::DefineGlobal:
            return ConstantInstruction("OP_DEFINE_GLOBAL", segment, offset);
        case Op::Divide:
            return SimpleInstruction("OP_DIVIDE", offset);
        case Op::Equal:
            return SimpleInstruction("OP_EQUAL", offset);
        case Op::Exponent:
            return SimpleInstruction("OP_EXPONENT", offset);
        case Op::False:
            return SimpleInstruction("OP_FALSE", offset);
        case Op::GetGlobal:
            return ConstantInstruction("OP_GET_GLOBAL", segment, offset);
        case Op::GetLocal:
            return AddressInstruction("OP_GET_LOCAL", segment, offset);
        case Op::Greater:
            return SimpleInstruction("OP_GREATER", offset);
        case Op::Increment:
            return SimpleInstruction("OP_INCREMENT", offset);
        case Op::Jump:
            return JumpInstruction("OP_JUMP", 1, segment, offset);
        case Op::JumpIfFalse:
            return JumpInstruction("OP_JUMP_IF_FALSE", 1, segment, offset);
        case Op::Less:
            return SimpleInstruction("OP_LESS", offset);
        case Op::Loop:
            return JumpInstruction("OP_LOOP", -1, segment, offset);
        case Op::Multiply:
            return SimpleInstruction("OP_MULTIPLY", offset);
        case Op::Negate:
            return SimpleInstruction("OP_NEGATE", offset);
        case Op::Nil:
            return SimpleInstruction("OP_NIL", offset);
        case Op::Not:
            return SimpleInstruction("OP_NOT", offset);
        case Op::Pop:
            return SimpleInstruction("OP_POP", offset);
        case Op::PopN:
            return ShortInstruction("OP_POPN", segment, offset);
        case Op::Print:
            return SimpleInstruction("OP_PRINT", offset);
        case Op::Remainder:
            return SimpleInstruction("OP_REMAINDER", offset);
        case Op::Return:
            return SimpleInstruction("OP_RETURN", offset);
        case Op::SetGlobal:
            return ConstantInstruction("OP_SET_GLOBAL", segment, offset);
        case Op::SetLocal:
            return AddressInstruction("OP_SET_LOCAL", segment, offset);
        case Op::Subtract:
            return SimpleInstruction("OP_SUBTRACT", offset);
        case Op::True:
            return SimpleInstruction("OP_TRUE", offset);
        default:
            std::cout << "Unknown opcode: " << op << "\n";
            return offset + 1;
    }
}

