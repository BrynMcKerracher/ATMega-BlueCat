#include <iostream>
#include <fstream>
#include <sstream>
#include "BlueCat/BlueCat.h"

int main(int argc, char** argv) {
    BlueCat::VirtualMachine::InterpretMode vmMode = BlueCat::VirtualMachine::InterpretMode::CompileAndInterpret;
    //If extra arguments are supplied check for flags
    for (int arg = 2; arg < argc; ++arg) {
        if (strcmp(argv[arg], "--mode") == 0) {
            if (arg + 1 < argc) {
                if (strcmp(argv[arg + 1], "compile") == 0) {
                    vmMode = BlueCat::VirtualMachine::InterpretMode::CompileOnly;
                    arg++;
                }
                else if (strcmp(argv[arg + 1], "interpret") == 0) {
                    vmMode = BlueCat::VirtualMachine::InterpretMode::InterpretOnly;
                    arg++;
                }
                else if (strcmp(argv[arg + 1], "full") == 0) {
                    vmMode = BlueCat::VirtualMachine::InterpretMode::CompileAndInterpret;
                    arg++;
                }
                else {
                    std::cout << "Error: Unrecognised mode: Should be one of 'compile', 'interpret' or 'full'.";
                    return -1;
                }
            }
            else {
                std::cout << "Error: mode not specified: Should be one of 'compile', 'interpret' or 'full'.";
                return -1;
            }
        }
        else {
            std::cout << "Error: Unrecognised flag '" << argv[arg] << "'\n";
            return -1;
        }
    }

    std::ifstream file(argv[1]);
    if (file.is_open()) {
        std::stringstream fileBuffer;
        fileBuffer << file.rdbuf();

        std::cout << "========================= BlueCat VM Start ============================\n";
        BlueCat::VirtualMachine vm;
        BlueCat::VirtualMachine::InterpretResult result = vm.Interpret(fileBuffer.str().c_str(), vmMode);
        switch (result) {
            case BlueCat::VirtualMachine::InterpretResult::Ok:           std::cout << "Interpretation successful\n"; break;
            case BlueCat::VirtualMachine::InterpretResult::CompileError: std::cout << "Compiler Error\n"; break;
            case BlueCat::VirtualMachine::InterpretResult::RuntimeError: std::cout << "Runtime Error\n"; break;
        }
        std::cout << "=======================================================================\n";
    }
    else {
        std::cout << "File Error: Unable to open input file!\n";
        return -1;
    }
    return 0;
}