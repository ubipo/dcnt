#pragma once

#include <cstddef>

template <typename K, size_t tableSize>
struct StaticCastHashFunc {
    unsigned long operator()(const K &key) const
    {
        return static_cast<unsigned long>(key) % tableSize;
    }
};
