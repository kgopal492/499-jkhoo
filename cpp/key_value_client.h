#ifndef CPP_KEY_VALUE_CLIENT_H_
#define CPP_KEY_VALUE_CLIENT_H_
#include <deque>
#include <string>

#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"
#include "key_value_client_interface.h"

// data structure behind the KeyValueStoreServiceImpl
class KeyValueClient : public KeyValueClientInterface {
 public:
  virtual ~KeyValueClient(){};
  // puts key value pair in store_, appending value if there is already a value associated with key
  void put(const std::string& key, const std::string& value);
  // gets values associated with the key
  const std::deque<std::string>& get(const std::string& key);
  // deletes values associated with the key
  void deletekey(const std::string& key);

 private:
  // models connection to an endpoint
  std::shared_ptr<grpc::Channel> channel_ = grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials());
  // stub to communicate with KeyValueStore
  std::unique_ptr<chirp::KeyValueStore::Stub> stub_ = chirp::KeyValueStore::NewStub(channel_);
};
#endif
