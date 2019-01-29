#include <map>
#include <deque>
#include <string>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"

// data structure behind the KeyValueStoreServiceImpl
class KeyValueStore final {
 public:
  // puts key value pair in store_, appending value if there is already a value associated with key
  void put(const std::string& key, const std::string& value);
  // gets values associated with the key
  const std::deque<std::string>& get(const std::string& key);
  // deletes values associated with the key
  void deletekey(const std::string& key);

 private:
  // Map to store the keys and values of the KeyValueStoreServiceImpl
  std::map<std::string, std::deque<std::string>> store_;
  // Protects store_
  std::mutex store_mut_;
  // separates values in `get()` return string
  const std::string delim = "4ba37ad81cc870aa40fb7bd9615a1b8a";
};
