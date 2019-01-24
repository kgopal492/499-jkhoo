#include "key_value_store.h"


void KeyValueStore::put(std::string key, std::string value) {
  std::lock_guard<std::mutex> lock(store_mut_);
  if (store_.find(key) == store_.end()) {
    std::list<std::string> initialElement;
    initialElement.push_back(value);
    store_[key] = initialElement;
  }
  else {
    store_[key].push_back(value);
  }
}

std::string KeyValueStore::get(std::string key) {
  // TODO: get whatever the request wants and set it as value in reply
  return "placeholder";
}

void KeyValueStore::deletekey(std::string key) {
  // TODO: delete what the request wants
}
