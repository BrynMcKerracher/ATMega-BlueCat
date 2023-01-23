/**
 * @file Memory.h
 * @author Bryn McKerracher
 */
#ifndef ATMEGA_BLUECAT_MEMORY_H
#define ATMEGA_BLUECAT_MEMORY_H

#include <stddef.h>
#include <stdlib.h>

///Defines the next capacity for a growing dynamic array. The array doubles in size each time.
#define GROW_CAPACITY(capacity) \
        ((capacity) < 4 ? 4 : (capacity) * 2)

///Reallocates memory for a dynamic array that is changing size.
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
        (type*)Reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

///Frees the memory for a dynamic array.
#define FREE_ARRAY(type, pointer, oldCount) \
        Reallocate(pointer, sizeof(type) * (oldCount), 0)

/**
 * @brief Reallocates memory for a dynamic array.
 * @param pointer A pointer to the array.
 * @param oldSize The current size of the array (in bytes).
 * @param newSize The new size of the array after the operation completes (in bytes).
 * @return
 * Performs 'realloc' on an array if newSize > 0, otherwise frees the array's memory.
 */
extern void* Reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif //ATMEGA_BLUECAT_MEMORY_H
