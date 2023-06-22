#ifndef BYTEBAC_STACK_H_INCLUDED
#define BYTEBAC_STACK_H_INCLUDED

#include <stdlib.h>
#include "Common.h"
#include "Memory.h"

#define STACK_GROWTH_FACTOR 2
#define STACK_MIN_ARRAY_SIZE 2

namespace BlueCat {
    template <typename T>
    class Stack {
        public:
            void Push(const T& element);
            T Pop();
            const T& Peek(Board::AddressOffset offset) const;
            const T& PeekBack() const;
            T& Peek(Board::AddressOffset offset);
            T& PeekBack();

            void PopNoAlloc(Board::SizeType numPops = 1);
            T& operator[](Board::SizeType index);
            const T& operator[](Board::SizeType index) const;
            Board::SizeType Count() const;
            void Resize(Board::SizeType newCapacity);
            void Clear();
            const T* GetArray() const;
        protected:
            Board::SizeType count = 0;
            Board::SizeType capacity = 0;
            T* array = nullptr;
    };
}

template <typename T>
void BlueCat::Stack<T>::Push(const T& element) {
    if (capacity == 0) {
        array = new T[STACK_MIN_ARRAY_SIZE];
        capacity = STACK_MIN_ARRAY_SIZE;
    }
    else if (count + 1 > capacity) {
        capacity *= STACK_GROWTH_FACTOR;
        T* copyArray = new T[capacity];
        memmove(copyArray, array, sizeof(T) * count);
        array = copyArray;
    }
    array[count++] = element;
}

template <typename T>
T BlueCat::Stack<T>::Pop() {
    return array[--count];
}

template <typename T>
const T& BlueCat::Stack<T>::Peek(Board::AddressOffset offset) const {
    return array[count - 1 + offset];
}

template <typename T>
const T& BlueCat::Stack<T>::PeekBack() const {
    return Peek(0);
}

template <typename T>
T& BlueCat::Stack<T>::Peek(Board::AddressOffset offset) {
    return array[count - 1 + offset];
}

template <typename T>
T& BlueCat::Stack<T>::PeekBack() {
    return Peek(0);
}

template <typename T>
void BlueCat::Stack<T>::PopNoAlloc(Board::SizeType numPops) {
    count -= numPops;
}

template <typename T>
T& BlueCat::Stack<T>::operator[](Board::SizeType index) {
    return array[index];
}

template <typename T>
const T& BlueCat::Stack<T>::operator[](Board::SizeType index) const {
    return array[index];
}

template <typename T>
BlueCat::Board::SizeType BlueCat::Stack<T>::Count() const {
    return count;
}

template <typename T>
void BlueCat::Stack<T>::Resize(Board::SizeType newCapacity) {
    if (capacity == 0 || array == nullptr) {
        array = new T[newCapacity];
        capacity = newCapacity;
        return;
    }

    T* newArray = new T[newCapacity];
    memcpy(newArray, array, capacity * sizeof(T));
    //Delete old data and update member variables
    delete [] array;
    array = newArray;
    capacity = newCapacity;
}

template <typename T>
void BlueCat::Stack<T>::Clear() {
    delete [] array;
    count = 0;
    capacity = 0;
    Resize(STACK_MIN_ARRAY_SIZE);
}

template <typename T>
const T* BlueCat::Stack<T>::GetArray() const {
    return array;
}

#undef STACK_GROWTH_FACTOR
#undef STACK_MIN_ARRAY_SIZE

#endif //BYTEBAC_STACK_H_INCLUDED
