//
// Created by Bryn McKerracher on 22/01/23.
//

#include "OpBatch.h"
#include "Debug.h"

int main(int argc, const char** argv) {
    OpBatch opBatch;
    InitOpBatch(&opBatch);
    WriteOpToBatch(&opBatch, RETURN);
    WriteOpToBatch(&opBatch, RETURN);
    WriteOpToBatch(&opBatch, RETURN);
    WriteOpToBatch(&opBatch, RETURN);
    DisassembleOpBatch(&opBatch, "Batch #1");
    FreeOpBatch(&opBatch);

    return 0;
}