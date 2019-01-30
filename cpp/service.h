#include <map>
#include <string>

#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "key_value_store.h"

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service from service.proto
class ServiceLayer final {
 public:
  // Communicates with KeyValueStoreServiceImpl to register username
  void registeruser(const std::string& username);
  // Communicates with KeyValueStoreServiceImpl to add a chirp
  chirp::Chirp chirp(const std::string& username, const std::string& text, const std::string& parent_id);
  // Communicates with KeyValueStoreServiceImpl to add to_follow to username's list of people following
  void follow(const std::string& username, const std::string& to_follow);
  // Communicates with KeyValueStoreServiceImpl to read from chirp_id
  chirp::Chirp read(const std::string& chirp_id);
  // Communicates with KeyValueStoreServiceImpl to stream chirps from the users username is following
  chirp::Chirp monitor(const std::string& username);

 private:
  // keeps track of the id of the next chirp
  int curr_id_ = 0;
  // models connection to an endpoint
  std::shared_ptr<grpc::Channel> channel_ = grpc::CreateChannel("localhost:50000", grpc::InsecureChannelCredentials());
  // stub to communicate with KeyValueStore
  std::unique_ptr<chirp::KeyValueStore::Stub> stub_ = chirp::KeyValueStore::NewStub(channel_);
  // determines whether to communicate over GRPC or just with the key value store
  bool debug_mode_ = true;
  // store for debugging mode
  KeyValueStore debug_store_;
};
