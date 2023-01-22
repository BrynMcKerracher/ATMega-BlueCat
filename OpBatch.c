//
// Created by Bryn McKerracher on 22/01/23.
//
#include "OpBatch.h"
#include "Definitions.h"

void InitOpBatch(OpBatch* opBatch) {
    opBatch->count = 0;
    opBatch->capacity = 0;
    opBatch->ops = BCNULL;
}