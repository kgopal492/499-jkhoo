#include <map>
#include <string>

#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service from service.proto
class ServiceLayer final {
 public:
  enum ActionEnum {
    kUserChirps = 0,
    kUserFollowing = 1,
    kChirpText = 2,
    kChirpReplies = 3,
  };
  void registeruser(std::string username);
  chirp::Chirp chirp(std::string username, std::string text, std::string parent_id);
  void follow(std::string username, std::string to_follow);
  chirp::Chirp read(std::string chirp_id);
  chirp::Chirp monitor(std::string username);

 private:
  // keeps track of the id of the next chirp
  int curr_id_ = 0;
};
