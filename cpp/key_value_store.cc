#include "key_value_store.h"


void KeyValueStore::put(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(store_mut_);
  store_[key].push_back(value);
}

std::string KeyValueStore::get(const std::string& key) {
  // TODO: get whatever the request wants and set it as value in reply
  return "placeholder";
}

void KeyValueStore::deletekey(const std::string& key) {
  // TODO: delete what the request wants
}
