/**
 * @file OpCode.h
 * @author Bryn McKerracher
 */
#ifndef ATMEGA_BLUECAT_OPCODE_H
#define ATMEGA_BLUECAT_OPCODE_H

#include <stdint.h> //For uint8_t

//Definition of an Op - Can change the underlying type to change Op size.
#define Op uint8_t

/**
 * @enum OpCode
 * @brief A comprehensive list of all ops supported by the VM.
 *
 * @note The number of supported ops should never exceed 2^(sizeof(Op) * 8)
 * @note OpCode has the __packed__ attribute to reduce the size of the OpCode enum from 4 bytes to 1 byte.
 */
typedef enum __attribute__ ((__packed__)) {
    RETURN ///< Function Return: Designates the end of a function frame.
} OpCode;

#endif //ATMEGA_BLUECAT_OPCODE_H
