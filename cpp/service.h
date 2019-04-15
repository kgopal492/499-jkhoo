#ifndef CPP_SERVICE_H_
#define CPP_SERVICE_H_
#include <chrono>
#include <map>
#include <mutex>
#include <string>

#include <google/protobuf/util/time_util.h>
#include <grpcpp/grpcpp.h>
#include "key_value_store.h"
#include "service.grpc.pb.h"

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service
// from service.proto
class ServiceLayer final {
 public:
  ServiceLayer(KeyValueClientInterface* key_value_connection);
  // Communicates with KeyValueStoreServiceImpl to register username. Returns
  // true if it is successful
  bool registeruser(const std::string& username);
  // Communicates with KeyValueStoreServiceImpl to add a chirp by given
  // username, with given text, and optionally as a reply to chirp with id
  // parent_id. Returns the newly saved chirp
  chirp::Chirp chirp(const std::string& username, const std::string& text,
                     const std::string& parent_id);
  // Communicates with KeyValueStoreServiceImpl to add to_follow to username's
  // list of people following. Returns true if it is successful
  bool follow(const std::string& username, const std::string& to_follow);
  // Communicates with KeyValueStoreServiceImpl to read chirps from chirp_id.
  // Returns a deque with the chirps
  std::deque<chirp::Chirp> read(const std::string& chirp_id);
  // Communicates with KeyValueStoreServiceImpl to stream chirps from the users
  // username is following by returning a deque of chirps created after start
  // time
  std::deque<chirp::Chirp> monitor(const std::string& username,
                                   chirp::Timestamp start);
  // Communicates with KeyValueStoreServiceImpl to stream chirps from the users
  // username is following by returning a deque of chirps created after start
  // time
  std::deque<chirp::Chirp> monitor(const std::string& username,
                                   chirp::Timestamp start);

 private:
  // helper function for chirp to create a Chirp message with the given
  // username, text, and parent_id
  chirp::Chirp chirpConstructionHelper(const std::string& username,
                                       const std::string& text,
                                       const std::string& parent_id);
  // connection to the KeyValueStore
  KeyValueClientInterface* store_;
  // constants for prepended values for categorizing keys
  // constant for the key that holds the ids of a user's chirps
  const std::string kuserChirps_ = "0";
  // constant for the key that holds who the user is following
  const std::string kuserFollowing_ = "1";
  // constant for the key that holds the value of a chirp
  const std::string kchirpValue_ = "2";
  // constant for the key that holds the ids of a chirp's replies
  const std::string kchirpReplies_ = "3";
  // constant for the key that holds the hashtags that are being followed
  const std::string khashtag_ = "4";
};
#endif
