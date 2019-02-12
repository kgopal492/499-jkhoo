#ifndef CPP_CLIENT_H_
#define CPP_CLIENT_H_
#include "service.grpc.pb.h"
#include "service.pb.h"
#include <iostream>

// class behind the client layer
class Client {
 public:
  // registers user, returns true if successful
  bool registeruser(std::string username, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // sends a chirp with the given information
  void sendchirp(std::string username, std::string text, std::string reply_id, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // adds to_follow to username's list of users following
  void follow(std::string username, std::string to_follow, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // reads the chirp string starting at id
  void read(std::string id, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
  // streams chirps from users username is following
  void monitor(std::string username, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_);
 private:
};
#endif
