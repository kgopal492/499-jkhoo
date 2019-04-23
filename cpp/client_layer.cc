#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>
#include <iostream>

#include "client.h"
#include "service.grpc.pb.h"
#include "service.pb.h"

DEFINE_string(register, "", "Register a username");
DEFINE_string(user, "", "Login a user");
DEFINE_string(chirp, "", "Send a chirp");
DEFINE_string(reply, "", "Make a chirp a reply to another chirp");
DEFINE_string(follow, "", "Follow a user");
DEFINE_string(read, "", "Read a chirp");
DEFINE_bool(monitor, false, "Stream chirps from followed users");
DEFINE_string(stream, "", "Stream chirps with a given hashtag");

// parses command line flags and triggers appropriate action
int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::shared_ptr<grpc::Channel> service_connection_ = grpc::CreateChannel(
      "localhost:50002", grpc::InsecureChannelCredentials());
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_ =
      chirp::ServiceLayer::NewStub(service_connection_);
  std::string my_username = "";
  bool logged_in_action_needed = false;
  Client client;
  if (FLAGS_register != "") {
    client.registeruser(FLAGS_register, stub_);
  }
  if (FLAGS_user != "") {
    my_username = FLAGS_user;
    logged_in_action_needed = true;
  }
  if (FLAGS_chirp != "" && my_username != "") {
    client.sendchirp(my_username, FLAGS_chirp, FLAGS_reply, stub_);
    logged_in_action_needed = false;
  }
  if (FLAGS_follow != "" && my_username != "") {
    client.follow(my_username, FLAGS_follow, stub_);
    logged_in_action_needed = false;
  }
  if (FLAGS_read != "") {
    client.read(FLAGS_read, stub_);
    logged_in_action_needed = false;
  }
  if (FLAGS_monitor && my_username != "") {
    client.monitor(my_username, stub_);
    logged_in_action_needed = false;
  }
  if (FLAGS_stream != "" && my_username != "") {
    client.stream(FLAGS_stream, my_username, stub_);
    logged_in_action_needed = false;
  }
  if (logged_in_action_needed) {
    std::cout << "You did not perform an action after logging in" << std::endl;
  }

  return 0;
}
