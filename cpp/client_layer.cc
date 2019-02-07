#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "service.pb.h"
//#include "backendStore.grpc.pb.h"
//#include "backendStore.pb.h"
#include <iostream>
//#include <memory>

int main(int argc, char** argv) {

  std::shared_ptr<grpc::Channel> service_connection_ = grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials());
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_ = chirp::ServiceLayer::NewStub(service_connection_);
  grpc::ClientContext context;
  chirp::RegisterRequest request;
  request.set_username("TestUser");
  chirp::RegisterReply reply;
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  std::cout<<status.error_message()<<std::endl;
  std::cout<<status.error_code()<<std::endl;
  std::cout<<reply.DebugString()<<std::endl;

  grpc::ClientContext chirpContext;
  chirp::ChirpRequest chirpRequest;
  chirp::ChirpReply chirpReply;
  chirpRequest.set_username("TestUser");
  chirpRequest.set_text("Test text");
  chirpRequest.set_parent_id("");
  grpc::Status chirpStat = stub_->chirp(&chirpContext, chirpRequest, &chirpReply);
  std::cout<<chirpStat.error_message()<<std::endl;
  std::cout<<chirpStat.error_code()<<std::endl;
  std::cout<<chirpReply.DebugString()<<std::endl;

  grpc::ClientContext registerContext;
  chirp::RegisterRequest registerRequest;
  chirp::RegisterReply registerReply;
  registerRequest.set_username("TestUser2");
  grpc::Status registerStatus = stub_->registeruser(&registerContext, registerRequest, &registerReply);
  std::cout<<registerStatus.error_message()<<std::endl;
  std::cout<<registerStatus.error_code()<<std::endl;
  std::cout<<registerReply.DebugString()<<std::endl;

  grpc::ClientContext followContext;
  chirp::FollowRequest followRequest;
  chirp::FollowReply followReply;
  followRequest.set_username("TestUser");
  followRequest.set_to_follow("TestUser2");
  grpc::Status followStatus = stub_->follow(&followContext, followRequest, &followReply);
  std::cout<<followStatus.error_message()<<std::endl;
  std::cout<<followStatus.error_code()<<std::endl;
  std::cout<<followReply.DebugString()<<std::endl;

  grpc::ClientContext readContext;
  chirp::ReadRequest readRequest;
  chirp::ReadReply readReply;
  readRequest.set_chirp_id("0");
  grpc::Status readStatus = stub_->read(&readContext, readRequest, &readReply);
  std::cout<<readStatus.error_message()<<std::endl;
  std::cout<<readStatus.error_code()<<std::endl;
  std::cout<<readReply.DebugString()<<std::endl;


  return 0;
}
