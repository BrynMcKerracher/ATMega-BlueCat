#ifndef BYTEBAC_HASHTABLE_H_INCLUDED
#define BYTEBAC_HASHTABLE_H_INCLUDED

#include "Common.h"
#include "ObjectString.h"
#include "Value.h"
#include "Memory.h"
#include "Stack.h"
#include <iostream>

namespace BlueCat {
    template <typename K, typename V>
    class HashTable {
    public:
        struct KVPair {
            K key;
            V value;

            bool occupied = false;
            bool tombstone = false;
        };

        HashTable();

        void Insert(const K& key, const V& value);
        bool Get(const K& key, V& outputValue);
        
        bool Set(const K& key, const V& newValue);
        void Remove(const K& key);
        void Print() const;
    protected:
        constexpr static Board::SizeType MinBuckets = 2;
        constexpr static float LoadFactor = 0.8f;
        constexpr static float GrowthFactor = 1.5f;

        Stack<KVPair> entries;
        Board::SizeType count = 0;
        Board::SizeType capacity = 0;

        void Rehash(Board::SizeType newCapacity);
        Board::SizeType Hash(const K& key) const;
    };
}

template <typename K, typename V>
BlueCat::HashTable<K, V>::HashTable() {
    Rehash(MinBuckets);
}

template <typename K, typename V>
void BlueCat::HashTable<K, V>::Insert(const K& key, const V& value) {
    if (count + 1 >= capacity * LoadFactor) {
        Rehash(capacity * GrowthFactor);
    }
    size_t index = Hash(key);
    while (entries[index].occupied) {
        if (entries[index].key == key && !entries[index].tombstone) {
            entries[index].value = value;
            return;
        }
        index = (index + 1) % capacity;
    }
    entries[index].key = key;
    entries[index].value = value;
    entries[index].occupied = true;
    entries[index].tombstone = false;    
    count++;
}

template <typename K, typename V>
bool BlueCat::HashTable<K, V>::Get(const K& key, V& outputValue) {
    size_t index = Hash(key);
    while (entries[index].occupied || entries[index].tombstone) {        
        if (*entries[index].key == *key && !entries[index].tombstone) {
            outputValue = entries[index].value;
            return true;
        }
        index = (index + 1) % capacity;
    }
    return false;
}

template <typename K, typename V>
bool BlueCat::HashTable<K, V>::Set(const K& key, const V& newValue) {
    size_t index = Hash(key);
    while (entries[index].occupied || entries[index].tombstone) {        
        if (*entries[index].key == *key && !entries[index].tombstone) {
            entries[index].value = newValue;
            return false;
        }
        index = (index + 1) % capacity;
    }
    Insert(key, newValue);
    return true;
}

template <typename K, typename V>
void BlueCat::HashTable<K, V>::Remove(const K& key) {
    size_t index = Hash(key);
    while (entries[index].occupied) {
        if (entries[index].key == key && !entries[index].tombstone) {
            //Set tombstone to indicate removed entry
            //We don't decrement count because count is the number of occupied entries PLUS tombstones
            entries[index].tombstone = true;
            return;
        }
        index = (index + 1) % capacity;
    }
}

template <typename K, typename V>
void BlueCat::HashTable<K, V>::Rehash(Board::SizeType newCapacity) {
    Stack<KVPair> oldEntries;
    
    size_t oldCapacity = capacity;
    for (size_t i = 0; i < oldCapacity; ++i) {
        oldEntries.Push(entries[i]);
    }

    count = 0;
    capacity = newCapacity;
    entries.Clear();
    entries.Resize(capacity);

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldEntries[i].occupied) {
            Insert(oldEntries[i].key, oldEntries[i].value);
        }
    }
}

template <typename K, typename V>
BlueCat::Board::SizeType BlueCat::HashTable<K, V>::Hash(const K& key) const {
    //FNV-1a Hash (for 32-bit hashing)
    uint32_t keyHash = 2166136261u;
    for (size_t i = 0; i < key->length; ++i) {
        keyHash ^= (uint8_t)(key->characters[i]);
        keyHash *= 16777619;
    }
    keyHash ^= (uint16_t)(keyHash >> 16); //Fold high bits into low bits, in-place.
    return keyHash % capacity;
}

template <typename K, typename V>
void BlueCat::HashTable<K, V>::Print() const {
    std::cout << "Count: " << count << "\n";
    std::cout << "Capacity: " << capacity << "\n";
    for (size_t i = 0; i < capacity; ++i) {
        if (!entries[i].occupied) continue;
        std::cout << "Entry (" << i << "):\n";
        std::cout << "\tKey: " << entries[i].key << "\n";
        std::cout << "\tValue: " << entries[i].value << "\n";
        std::cout << "\tOccupied: " << (entries[i].occupied ? "true" : "false") << "\n";
        std::cout << "\tTombstone: " << (entries[i].tombstone ? "true" : "false") << "\n";
    }
}
#endif //BYTEBAC_HASHTABLE_H_INCLUDED
