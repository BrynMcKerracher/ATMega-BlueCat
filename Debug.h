/**
 * @file Debug.h
 * @author Bryn McKerracher
 */
#ifndef ATMEGA_BLUECAT_DEBUG_H
#define ATMEGA_BLUECAT_DEBUG_H

#include "OpBatch.h" //For OpBatch

/**
 * @brief Prints the opcodes from an OpBatch alongside additional debug information.
 * @param opBatch The OpBatch to disassemble.
 * @param name The name of the OpBatch.
 */
extern void DisassembleOpBatch(OpBatch* opBatch, const char* name);

/**
 * @brief Prints the name the opcode found in the given OpBatch at location 'offset'.
 * @param opBatch The OpBatch containing the instruction for disassembly.
 * @param offset The index of the instruction in the given OpBatch.
 * @return The index of the next opcode to be disassembled in the given OpBatch.
 */
extern int DisassembleInstruction(OpBatch* opBatch, int offset);

#endif //ATMEGA_BLUECAT_DEBUG_H
