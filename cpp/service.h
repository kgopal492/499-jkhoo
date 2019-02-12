#ifndef CPP_SERVICE_H_
#define CPP_SERVICE_H_
#include <map>
#include <string>
#include <mutex>
#include <chrono>

#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "key_value_store.h"
#include <google/protobuf/util/time_util.h>

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service from service.proto
class ServiceLayer final {
 public:
  ServiceLayer(KeyValueClientInterface* key_value_connection);
  // Communicates with KeyValueStoreServiceImpl to register username
  bool registeruser(const std::string& username);
  // Communicates with KeyValueStoreServiceImpl to add a chirp
  chirp::Chirp chirp(const std::string& username, const std::string& text, const std::string& parent_id);
  // Communicates with KeyValueStoreServiceImpl to add to_follow to username's list of people following
  bool follow(const std::string& username, const std::string& to_follow);
  // Communicates with KeyValueStoreServiceImpl to read from chirp_id
  std::deque<chirp::Chirp> read(const std::string& chirp_id);
  // Communicates with KeyValueStoreServiceImpl to stream chirps from the users username is following by returning chirps after start time
  std::deque<chirp::Chirp> monitor(const std::string& username, chirp::Timestamp start);

 private:
  // connection to the KeyValueStore
  KeyValueClientInterface* store_;
  // constants for prepended values for categorizing keys
  const std::string kuserChirps_ = "0";
  const std::string kuserFollowing_ = "1";
  const std::string kchirpValue_ = "2";
  const std::string kchirpReplies_ = "3";
};
#endif
