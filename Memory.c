//
// Created by Bryn McKerracher on 23/01/23.
//
#include "Memory.h"

void* Reallocate(void* pointer, size_t oldSize, size_t newSize) {
    //If desired size is 0, free memory.
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    //Otherwise attempt realloc, and exit(1) if there's a problem.
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}