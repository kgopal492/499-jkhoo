#include <map>
#include <list>
#include <string>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"

// data structure behind the KeyValueStoreServiceImpl
class KeyValueStore final {
 public:
  void put(std::string key, std::string value);
  std::string get(std::string key);
  void deletekey(std::string key);

 private:
  // Map to store the keys and values of the KeyValueStoreServiceImpl
  std::map<std::string, std::list<std::string>> store_;
  std::mutex store_mut_;
};
