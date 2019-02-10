#include "key_value_store.h"


void KeyValueStore::put(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(store_mut_);
  store_[key].push_back(value);
}

std::deque<std::string> KeyValueStore::get(const std::string& key) {
  std::lock_guard<std::mutex> lock(store_mut_);
  std::deque<std::string> values = store_[key];
  return values;
}

void KeyValueStore::deletekey(const std::string& key) {
  std::lock_guard<std::mutex> lock(store_mut_);
  store_.erase(key);
}
