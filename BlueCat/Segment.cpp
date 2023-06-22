#include "Segment.h"
#include "Compiler/Serialiser.h"
#include "Compiler/Deserialiser.h"
#include <fstream>

BlueCat::Board::SizeType BlueCat::Segment::AddConstant(const Value& value) {
    constants.Push(value);
    return constants.Count() - 1;
}

BlueCat::Value BlueCat::Segment::GetConstant(Board::SizeType index) {
    return constants[index];
}

void BlueCat::Segment::AddOp(Op op) {
    instructions.Push(op);
}

BlueCat::Board::Address BlueCat::Segment::ReadAddress(Board::SizeType index) {
    BlueCat::Board::Address address = 0;
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerAddress; ++i) {
        address |= ((BlueCat::Board::Address)instructions[index + i]) << (8 * sizeof(Op) * i);
    }
    return address;
}

BlueCat::Board::ShortAddress BlueCat::Segment::ReadShortAddress(Board::SizeType index) {
    BlueCat::Board::ShortAddress address = 0;
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerShortAddress; ++i) {
        address |= ((BlueCat::Board::Address)instructions[index + i]) << (8 * sizeof(Op) * i);
    }
    return address;
}

bool BlueCat::Segment::WriteToFile(const char* name) const {
    std::ofstream outputAssembly(name, std::ios::binary);
    if (outputAssembly.is_open()) {
        for (size_t i = 0; i < instructions.Count(); ++i) {
            outputAssembly << Serialiser::BytesToHexString(instructions[i]);
        }
        outputAssembly << "\n";
        for (size_t i = 0; i < constants.Count(); ++i) {
            outputAssembly << Serialiser::ValueToHexString(constants[i]);
        }
        return true;
    }
    return false;
}

bool BlueCat::Segment::LoadFromFile(const char* name) {
    std::ifstream inputAssembly(name, std::ios::binary);
    if (inputAssembly.is_open()) {
        std::string line;
        std::getline(inputAssembly, line);
        for (size_t i = 0; i < line.size(); i += 2) {
            AddOp(Deserialiser::HexCharsToByte<Op>(line[i], line[i + 1]));
        }
        while (std::getline(inputAssembly, line)) { //Add values
            AddConstant(Deserialiser::HexStringToValue(line.c_str()));
        }
        return true;
    }
    return false;
}

BlueCat::Board::SizeType BlueCat::Segment::InstructionCount() const {
    return instructions.Count();
}