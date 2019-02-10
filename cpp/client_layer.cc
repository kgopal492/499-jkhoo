#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "service.pb.h"
#include <iostream>
#include <gflags/gflags.h>
DEFINE_string(register, "", "Register a username");
DEFINE_string(user, "", "Login a user");
DEFINE_string(chirp, "", "Send a chirp");
DEFINE_string(reply, "", "Make a chirp a reply to another chirp");
DEFINE_string(follow, "", "Follow a user");
DEFINE_string(read, "", "Read a chirp");
DEFINE_bool(monitor, false, "Stream chirps from followed users");
bool registeruser(std::string username, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  if(username.length()==0){
    std::cout<<"Must enter non empty username"<<std::endl;
    return false;
  }
  grpc::ClientContext context;
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  //std::cout<<status.error_message()<<std::endl;
  //std::cout<<status.error_code()<<std::endl;
  //std::cout<<reply.DebugString()<<std::endl;
  if(status.error_code()!=0){
    std::cout<<"Something went wrong :("<<std::endl;
    return false;
  }
  std::cout<<"User "<<username<<" registered!"<<std::endl;
  return true;
}

bool loginuser(std::string username, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  grpc::ClientContext context;
  chirp::RegisterRequest request;
  request.set_username(username);
  chirp::RegisterReply reply;
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  //std::cout<<status.error_message()<<std::endl;
  //std::cout<<status.error_code()<<std::endl;
  //std::cout<<reply.DebugString()<<std::endl;
  if(status.error_code()!=3){
    std::cout<<"Something went wrong :("<<std::endl;
    return false;
  }
  std::cout<<"User "<<username<<" logged in!"<<std::endl;
  return true;
}

bool sendchirp(std::string username, std::string text, std::string reply_id, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  grpc::ClientContext context;
  chirp::ChirpRequest request;
  chirp::ChirpReply reply;
  request.set_username(username);
  request.set_text(text);
  request.set_parent_id(reply_id);

  grpc::Status status = stub_->chirp(&context, request, &reply);
  if(status.error_code()!=0){
    std::cout<<"Something went wrong :("<<std::endl;
    if(reply_id.length()>0){
      std::cout<<"Make sure the parent id actually exists"<<std::endl;
    }
    return false;
  }
  //std::cout<<status.error_message()<<std::endl;
  //std::cout<<status.error_code()<<std::endl;
  //std::cout<<reply.DebugString()<<std::endl;
  std::cout<<"Chirp with chirp id "<<reply.chirp().id()<<" created"<<std::endl;
  return true;
}
bool follow(std::string username, std::string to_follow, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  if(to_follow.length() == 0){
    std::cout<<"Must enter non empty user to follow"<<std::endl;
    return false;
  }
  grpc::ClientContext context;
  chirp::FollowRequest request;
  chirp::FollowReply reply;
  request.set_username(username);
  request.set_to_follow(to_follow);
  grpc::Status status = stub_->follow(&context, request, &reply);
  //std::cout<<status.error_message()<<std::endl;
  //std::cout<<status.error_code()<<std::endl;
  //std::cout<<reply.DebugString()<<std::endl;
  if(status.error_code()!=0){
    std::cout<<"Something went wrong :("<<std::endl;
    return false;
  }
  std::cout<<"User "<<username<<" now following "<<to_follow<<std::endl;
  return true;
}
bool read(std::string id, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  grpc::ClientContext context;
  chirp::ReadRequest request;
  chirp::ReadReply reply;
  request.set_chirp_id(id);
  grpc::Status status = stub_->read(&context, request, &reply);
  //std::cout<<status.error_message()<<std::endl;
  //std::cout<<status.error_code()<<std::endl;
  //std::cout<<reply.DebugString()<<std::endl;
  if(status.error_code()!=0){
    std::cout<<"Something went wrong :("<<std::endl;
    return false;
  }
  int num_chirps = reply.chirps_size();
  for(int i = 0; i<num_chirps - 1; i++){
    chirp::Chirp this_chirp = reply.chirps(i);
    std::cout<<this_chirp.username()<<": "<<this_chirp.text()<<std::endl;
  }
  return true;
}
bool monitor(std::string username, std::unique_ptr<chirp::ServiceLayer::Stub>& stub_){
  std::cout<<"PLEASE"<<std::endl;
  grpc::ClientContext context;
  chirp::MonitorRequest request;
  request.set_username(username);
  chirp::MonitorReply reply;
  std::unique_ptr<grpc::ClientReader<chirp::MonitorReply> > stream_handle (stub_->monitor(&context, request));
  while(true){
    while(stream_handle->Read(&reply)){
      chirp::Chirp this_chirp = reply.chirp();
      std::cout<<this_chirp.username()<<": "<<this_chirp.text()<<std::endl;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::shared_ptr<grpc::Channel> service_connection_ = grpc::CreateChannel("localhost:50002", grpc::InsecureChannelCredentials());
  std::unique_ptr<chirp::ServiceLayer::Stub> stub_ = chirp::ServiceLayer::NewStub(service_connection_);
  std::string my_username="";
  if(FLAGS_register != ""){
    registeruser(FLAGS_register, stub_);
  }
  if(FLAGS_user != ""){
    bool loggedin = loginuser(FLAGS_user, stub_);
    if(loggedin){
      my_username=FLAGS_user;
    }
  }
  if(FLAGS_chirp != "" && my_username != ""){
    sendchirp(my_username, FLAGS_chirp, FLAGS_reply, stub_);
  }
  if(FLAGS_follow != "" && my_username != ""){
    follow(my_username, FLAGS_follow, stub_);
  }
  if(FLAGS_read != "" && my_username != ""){
    read(FLAGS_read, stub_);
  }
  if(FLAGS_monitor){
    monitor(my_username, stub_);
  }
  //bool do_register = (FLAGS_register != "");
  //std::cout<<FLAGS_register<<std::endl;
  /*grpc::ClientContext context;
  chirp::RegisterRequest request;
  request.set_username("TestUser");
  chirp::RegisterReply reply;
  grpc::Status status = stub_->registeruser(&context, request, &reply);
  std::cout<<status.error_message()<<std::endl;
  std::cout<<status.error_code()<<std::endl;
  std::cout<<reply.DebugString()<<std::endl;*/
  /*bool registerSuccess = registeruser("TestUser", stub_);
  bool loginSuccess = loginuser("TestUser", stub_);
  std::cout<<registerSuccess<<" "<<loginSuccess<<std::endl;
  sendchirp("TestUser", "hello world", "", stub_);
  sendchirp("TestUser", "hello world again", "0", stub_);
  registeruser("TestUser2", stub_);
  follow("TestUser", "TestUser2", stub_);
  follow("TestUser", "", stub_);
  read("1", stub_);
  read("0", stub_);*/
/*
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
  std::cout<<readReply.DebugString()<<std::endl;*/


  return 0;
}
