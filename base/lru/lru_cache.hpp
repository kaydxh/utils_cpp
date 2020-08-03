//
// Created by kayxhding on 2020-07-09 20:29:20
//

#ifndef KINGFISHER_BASE_LRU_LRU_CACHE_H_
#define KINGFISHER_BASE_LRU_LRU_CACHE_H_

#include <list>
#include <mutex>
#include <unordered_map>

namespace kingfisher {
namespace lru {

template <typename KeyType, typename ValueType>
class LruCache {
 public:
  using KeyValuePairType = typename std::pair<KeyType, ValueType>;
  using ListIteratorType = typename std::list<KeyValuePairType>::iterator;

  LruCache(size_t capacity);
  void Put(const KeyType &key, const ValueType &value);
  bool Get(const KeyType &key, ValueType &value);

 private:
  std::mutex mutex_;
  size_t capacity_;
  // lru cache, save visited elements, last visited element put front, first
  // visited element put back
  std::list<KeyValuePairType> cache_items_list_;
  // key -> iterator, through key, can visit element from list fastly
  std::unordered_map<KeyType, ListIteratorType> cache_items_map_;
};

template <typename KeyType, typename ValueType>
LruCache<KeyType, ValueType>::LruCache(size_t capacity) : capacity_(capacity) {}

template <typename KeyType, typename ValueType>
void LruCache<KeyType, ValueType>::Put(const KeyType &key,
                                       const ValueType &value) {
  std::unique_lock<std::mutex> lock(mutex_);

  auto it = cache_items_map_.find(key);
  if (it != cache_items_map_.end()) {
    cache_items_list_.erase(it->second);
    cache_items_map_.erase(it);
  }

  cache_items_list_.push_front(KeyValuePairType(key, value));
  cache_items_map_[key] = cache_items_list_.begin();

  if (cache_items_map_.size() > capacity_) {
    cache_items_map_.erase(cache_items_list_.back().first);
    cache_items_list_.pop_back();
  }
}

template <typename KeyType, typename ValueType>
bool LruCache<KeyType, ValueType>::Get(const KeyType &key, ValueType &value) {
  std::unique_lock<std::mutex> lock_(mutex_);

  auto it = cache_items_map_.find(key);
  if (it == cache_items_map_.end()) {
    return false;
  }
  cache_items_list_.splice(cache_items_list_.begin(), cache_items_list_,
                           it->second);
  value = it->second->second;
  return true;
}

}  // namespace lru
}  // namespace kingfisher
#endif
