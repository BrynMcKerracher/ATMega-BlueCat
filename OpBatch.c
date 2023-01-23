//
// Created by Bryn McKerracher on 22/01/23.
//
#include "OpBatch.h"
#include "Memory.h"

void InitOpBatch(OpBatch* opBatch) {
    //Reset the OpBatch to the 'empty' state.
    opBatch->count = 0;
    opBatch->capacity = 0;
    opBatch->ops = NULL;
}

void WriteOpToBatch(OpBatch* opBatch, Op op) {
    //If we exceed the array's current capacity, grow the array.
    if (opBatch->capacity < opBatch->count + 1) {
        int oldCapacity = opBatch->count;
        opBatch->capacity = GROW_CAPACITY(oldCapacity);
        opBatch->ops = GROW_ARRAY(Op, opBatch->ops, oldCapacity, opBatch->capacity);
    }
    //Write the op, increment the op counter.
    opBatch->ops[opBatch->count++] = op;
}

void FreeOpBatch(OpBatch* opBatch) {
    //Free the array in memory, then reset the OpBatch to the 'empty' state.
    FREE_ARRAY(Op, opBatch->ops, opBatch->capacity);
    InitOpBatch(opBatch);
}