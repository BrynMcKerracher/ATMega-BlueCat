#include "Debug.h"
#include <printf.h> //For printf()

/**
 * @brief Prints the name of a one-code instruction and returns the index of the next location to be disassembled.
 * @param name The name of the opcode.
 * @param offset The index of the current opcode in the control flow.
 * @return The index for the next opcode in the control flow to be disassembled.
 */
static int SimpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

void DisassembleOpBatch(OpBatch* opBatch, const char* name) {
    printf("========= %s =========\n", name);
    for (int offset = 0; offset < opBatch->count; offset = DisassembleInstruction(opBatch, offset)) {}
}

int DisassembleInstruction(OpBatch* opBatch, int offset) {
    printf("%04d ", offset);
    Op instruction = opBatch->ops[offset];
    switch (instruction) {
        case RETURN: return SimpleInstruction("RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
