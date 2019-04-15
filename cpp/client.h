#ifndef CPP_CLIENT_H_
#define CPP_CLIENT_H_
#include <iostream>
#include "service.grpc.pb.h"
#include "service.pb.h"

// class behind the client layer
class Client {
 public:
  // registers user with given username, returns true if successful
  bool registeruser(std::string username,
                    std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // sends a chirp by given user with given text. Optionally as a reply to a
  // chirp with given reply_id
  void sendchirp(std::string username, std::string text, std::string reply_id,
                 std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // adds to_follow to username's list of users following
  void follow(std::string username, std::string to_follow,
              std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // reads the chirp string starting at id
  void read(std::string id, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // streams chirps from users username is following
  void monitor(std::string username,
               std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // streams chirps made containing a provided hashtag
  void stream(std::string hashtag,
               std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);

 private:
};
#endif
