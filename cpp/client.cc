#include "client.h"

bool Client::registeruser(std::string username,
                          std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  if (username.length() == 0) {
    std::cout << "Must enter non empty username" << std::endl;
    return false;
  }
  grpc::ClientContext context;
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  if (status.error_code() != 0) {
    std::cout << "Something went wrong :(" << std::endl;
    return false;
  }
  std::cout << "User " << username << " registered!" << std::endl;
  return true;
}

void Client::sendchirp(std::string username, std::string text,
                       std::string reply_id,
                       std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  grpc::ClientContext context;
  chirp::ChirpRequest request;
  chirp::ChirpReply reply;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(reply_id);

  grpc::Status status = stub_->chirp(&context, request, &reply);
  if (status.error_code() != 0) {
    std::cout << "Something went wrong :(" << std::endl;
    if (reply_id.length() > 0) {
      std::cout << "Make sure the parent id actually exists" << std::endl;
    }
  } else {
    std::cout << "Chirp with chirp id " << reply.chirp().id() << " created"
              << std::endl;
  }
}

void Client::follow(std::string username, std::string to_follow,
                    std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  if (to_follow.length() == 0) {
    std::cout << "Must enter non empty user to follow" << std::endl;
  } else {
    grpc::ClientContext context;
    chirp::FollowRequest request;
    chirp::FollowReply reply;
    request.set_username(username);
    request.set_to_follow(to_follow);
    grpc::Status status = stub_->follow(&context, request, &reply);
    if (status.error_code() != 0) {
      std::cout << "Something went wrong :(" << std::endl;
    } else {
      std::cout << "User " << username << " now following " << to_follow
                << std::endl;
    }
  }
}

void Client::read(std::string id,
                  std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  grpc::ClientContext context;
  chirp::ReadRequest request;
  chirp::ReadReply reply;
  request.set_chirp_id(id);
  grpc::Status status = stub_->read(&context, request, &reply);
  if (status.error_code() != 0) {
    std::cout << "Something went wrong :(" << std::endl;
  } else {
    int num_chirps = reply.chirps_size();
    chirp::Chirp this_chirp;
    for (int i = 0; i < num_chirps - 1; i++) {
      this_chirp = reply.chirps(i);
      std::cout << this_chirp.username() << ": " << this_chirp.text()
                << std::endl;
    }
  }
}

void Client::monitor(std::string username,
                     std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  grpc::ClientContext context;
  chirp::MonitorRequest request;
  request.set_username(username);
  chirp::MonitorReply reply;
  std::unique_ptr<grpc::ClientReader<chirp::MonitorReply> > stream_handle(
      stub_->monitor(&context, request));
  while (true) {
    chirp::Chirp this_chirp;
    while (stream_handle->Read(&reply)) {
      this_chirp = reply.chirp();
      std::cout << this_chirp.username() << ": " << this_chirp.text()
                << std::endl;
    }
  }
}

void Client::stream(std::string hashtag, std::string username,
                    std::unique_ptr<chirp::ServiceLayer::Stub>& stub_) {
  grpc::ClientContext context;
  chirp::StreamRequest request;
  request.set_hashtag(hashtag);
  request.set_username(username);
  chirp::StreamReply reply;
  std::unique_ptr<grpc::ClientReader<chirp::StreamReply> > stream_handle(
      stub_->stream(&context, request));
  // continues requesting chirps until program is quit
  // and connection is closed or if invalid username or hashtag is entered
  bool keep_streaming = true;
  while (keep_streaming) {
    chirp::Chirp this_chirp;
    while (stream_handle->Read(&reply)) {
      this_chirp = reply.chirp();
      std::cout << this_chirp.username() << ": " << this_chirp.text()
                << std::endl;
    }
    if(stream_handle->Finish().error_code() != 0) {
      std::cout << "Invalid username or hashtag for stream" << std::endl;
      keep_streaming = false;
    }
  }
}
