// ============================================================================
// FILE: Cache.h
// Description: Template-based LRU cache for recent user searches
// ============================================================================

#ifndef CACHE_H
#define CACHE_H

#include "common.h"
#include <list>
#include <unordered_map>

// LRU Cache template for storing recently searched items
template<typename K, typename V>
class LRUCache {
private:
    int capacity;
    std::list<std::pair<K, V>> cacheList;  // Most recent at front
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cacheMap;

public:
    // Constructor
    explicit LRUCache(int cap) : capacity(cap) {}

    // Get value from cache, returns nullptr if not found
    V* get(const K& key) {
        auto it = cacheMap.find(key);
        if (it == cacheMap.end()) {
            return nullptr;  // Cache miss
        }

        // Move to front (most recently used)
        cacheList.splice(cacheList.begin(), cacheList, it->second);
        return &(it->second->second);
    }

    // Put key-value pair in cache
    void put(const K& key, const V& value) {
        auto it = cacheMap.find(key);
        
        if (it != cacheMap.end()) {
            // Update existing entry and move to front
            it->second->second = value;
            cacheList.splice(cacheList.begin(), cacheList, it->second);
            return;
        }

        // Add new entry at front
        if (cacheList.size() >= static_cast<size_t>(capacity)) {
            // Remove least recently used (back)
            auto last = cacheList.back();
            cacheMap.erase(last.first);
            cacheList.pop_back();
        }

        cacheList.emplace_front(key, value);
        cacheMap[key] = cacheList.begin();
    }

    // Clear cache
    void clear() {
        cacheList.clear();
        cacheMap.clear();
    }

    // Get size
    size_t size() const {
        return cacheList.size();
    }
};

#endif // CACHE_H