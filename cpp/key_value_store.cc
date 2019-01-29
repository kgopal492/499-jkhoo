#include "key_value_store.h"


void KeyValueStore::put(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(store_mut_);
  store_[key].push_back(value);
}

const std::deque<std::string>& KeyValueStore::get(const std::string& key) {
  std::lock_guard<std::mutex> lock(store_mut_);
  const std::deque<std::string>& values = store_[key];
  return values;
}

void KeyValueStore::deletekey(const std::string& key) {
  store_.erase(key);
}
