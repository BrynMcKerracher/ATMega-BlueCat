/**
 * @file Value.h
 * @author Bryn McKerracher
 */
#ifndef ATMEGA_BLUECAT_VALUE_H
#define ATMEGA_BLUECAT_VALUE_H

/**
 * @brief Alias for the value types used by the VM.
 * @note Currently BlueCat is monotyped, but this will be expanded in the future.
 */
typedef double Value;

/**
 * @struct ValueArray
 * @brief A dynamic array of type 'Value'.
 */
typedef struct {
    int count;     ///< The current number of ops in the array.
    int capacity;  ///< The maximum number of ops in the array before a realloc occurs.
    Value* values; ///< Array of values.
} ValueArray;

/**
 * @brief Initialises a ValueArray to the 'empty' state.
 * @param array The array to initialise.
 * @note The 'empty' state refers to the state where the ValueArray has a capacity and count of zero, and it's
 * value array is null.
 */
extern void InitValueArray(ValueArray* array);

/**
 * @brief Appends a Value to the ValueArray.
 * @param array The ValueArray to append to.
 * @param value The Value to append.
 *
 * May cause a realloc if the array's count exceeds its capacity.
 */
extern void WriteToValueArray(ValueArray* array, Value value);

/**
 * @brief Frees the memory from the ValueArray's value array and sets the ValueArray to the 'empty' state.
 * @param array The array to be freed.
 */
extern void FreeValueArray(ValueArray* array);

#endif //ATMEGA_BLUECAT_VALUE_H
