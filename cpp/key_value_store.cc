#include "key_value_store.h"


void KeyValueStore::put(const std::string& key, const std::string& value) {
  std::lock_guard<std::mutex> lock(store_mut_);
  store_[key].push_back(value);
}

std::string KeyValueStore::get(const std::string& key) {
  // TODO: get whatever the request wants and set it as value in reply
  std::lock_guard<std::mutex> lock(store_mut_);
  std::deque<std::string> values = store_[key];
  std::string returnVal = "";
  for(std::string value: values){
    returnVal = returnVal+value+delim;
  }
  return returnVal;
}

void KeyValueStore::deletekey(const std::string& key) {
  // TODO: delete what the request wants
}
