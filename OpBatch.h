/**
 * @file OpBatch.h
 * @author Bryn McKerracher
 */

#ifndef ATMEGA_BLUECAT_OPBATCH_H
#define ATMEGA_BLUECAT_OPBATCH_H

#include <stdint.h> //For uint8_t
#include <stdlib.h> //For NULL
#include "OpCode.h" //For Op definition

/**
 * @struct OpBatch
 * @brief A dynamic array of OpCodes for processing by the VM.
 */
typedef struct {
    int count;    ///< The current number of ops in the array.
    int capacity; ///< The maximum number of ops in the array before a realloc occurs.
    Op* ops;      ///< Array of ops.
} OpBatch;

/**
 * @brief Initialises a batch to a well-defined 'empty' state.
 * @param opBatch The batch to initialise.
 *
 * Initialises an OpBatch count/capacity to zero, and sets its ops array to null.
 */
extern void InitOpBatch(OpBatch* opBatch);

/**
 * @brief Appends a single opcode to the OpBatch.
 * @param opBatch The OpBatch to write to.
 * @param op The opcode to be written to the batch.
 */
extern void WriteOpToBatch(OpBatch* opBatch, Op op);

/**
 * @brief Frees the data from an OpBatch and returns it to an empty state.
 * @param opBatch The OpBatch to free.
 */
extern void FreeOpBatch(OpBatch* opBatch);

#endif //ATMEGA_BLUECAT_OPBATCH_H
