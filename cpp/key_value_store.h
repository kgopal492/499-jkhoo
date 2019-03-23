#ifndef CPP_KEY_VALUE_STORE_H_
#define CPP_KEY_VALUE_STORE_H_
#include <deque>
#include <map>
#include <mutex>
#include <string>

#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"
#include "key_value_client_interface.h"

// data structure behind the KeyValueStoreServiceImpl
class KeyValueStore : public KeyValueClientInterface {
 public:
  virtual ~KeyValueStore(){};
  // puts key value pair in store_, appending value if there is already a value
  // associated with key
  void put(const std::string& key, const std::string& value);
  // gets values associated with the key
  std::deque<std::string> get(const std::string& key);
  // deletes values associated with the key
  void deletekey(const std::string& key);

 private:
  // Map to store the keys and values of the KeyValueStoreServiceImpl
  std::map<std::string, std::deque<std::string>> store_;
  // Protects store_
  std::mutex store_mut_;
};
#endif
