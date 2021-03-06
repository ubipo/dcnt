#pragma once

/**
 * Copyright 2017 HashMap Development Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "HashNode.h"
#include "ReinterpretCastHashFunc.h"
#include "KeyCompFunc.h"
#include "SingleRet.h"
#include <cstddef>


// Hash map class template
template <typename K, typename V, size_t tableSize, typename H = ReinterpretCastHashFunc<K, tableSize>, typename C = KeyCompFunc<K>>
class HashMap
{
public:
    HashMap() :
        table(),
        hashFunc()
    {
    }

    ~HashMap()
    {
        // destroy all buckets one by one
        for (size_t i = 0; i < tableSize; ++i) {
            HashNode<K, V> *entry = table[i];

            while (entry != nullptr) {
                HashNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }

            table[i] = nullptr;
        }
    }

    SingleRet<V> get(const K &key)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr) {
            if (compFunc(entry->getKey(), key)) {
                V value = entry->getValue();
                return SingleRet<V>{value, ERR_OK};
            }

            entry = entry->getNext();
        }

        return SingleRet<V>{0, ERR_FAIL};;
    }

    void put(const K &key, const V &value)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = nullptr;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr && !compFunc(entry->getKey(), key)) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            entry = new HashNode<K, V>(key, value);

            if (prev == nullptr) {
                // insert as first bucket
                table[hashValue] = entry;

            } else {
                prev->setNext(entry);
            }

        } else {
            // just update the value
            entry->setValue(value);
        }
    }

    void remove(const K &key)
    {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = nullptr;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr && !compFunc(entry->getKey(), key)) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            // key not found
            return;

        } else {
            if (prev == nullptr) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();

            } else {
                prev->setNext(entry->getNext());
            }

            delete entry;
        }
    }

private:
    HashMap(const HashMap & other);
    const HashMap & operator=(const HashMap & other);
    // hash table
    HashNode<K, V> *table[tableSize];
    H hashFunc;
    C compFunc;
};
