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
  return 0;
}
