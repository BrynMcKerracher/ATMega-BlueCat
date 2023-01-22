//
// Created by Bryn McKerracher on 22/01/23.
//

#ifndef ATMEGA_BLUECAT_OPBATCH_H
#define ATMEGA_BLUECAT_OPBATCH_H

#include <stdint.h> //For uint8_t

/**
 * @struct OpBatch
 * @brief A dynamic array of OpCodes for processing by the VM.
 *
 * Each opcode is exactly 1 byte (sizeof(uint8_t)).
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* ops;
} OpBatch;

void InitOpBatch(OpBatch* opBatch);

#endif //ATMEGA_BLUECAT_OPBATCH_H
