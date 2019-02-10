#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  std::cout<<"In register user"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  std::string username = request->username();
  bool response = service_.registeruser(username);
  if(!response){
    grpc::Status existing_user = grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Usernames must be unique");
    return existing_user;
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  std::cout<<"In chirp"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  chirp::Chirp this_chirp = service_.chirp(request->username(), request->text(), request->parent_id());
  chirp::Chirp* reply_chirp = reply->mutable_chirp();
  *reply_chirp = this_chirp;
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServiceImpl::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) {
  std::cout<<"In follow"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  service_.follow(request->username(), request->to_follow());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) {
  std::cout<<"In read"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  std::cout<<"Here we go"<<std::endl;
  std::deque<chirp::Chirp> read_chirps = service_.read(request->chirp_id());
  for(chirp::Chirp c : read_chirps){
    chirp::Chirp* chirp_pointer = reply->add_chirps();
    const chirp::Chirp& added_chirp = c;
    chirp_pointer->CopyFrom(added_chirp);
  }
  chirp::Chirp* chirp_pointer = reply->add_chirps();

  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, grpc::ServerWriter< ::chirp::MonitorReply>* stream) {
  //chirp::MonitorRequest request;
  //stream->Read(&request);
  std::cout<<"In monitor"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;

  chirp::Timestamp initial_time;
  std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::system_clock::now().time_since_epoch());
  initial_time.set_seconds(seconds.count());
  initial_time.set_useconds(useconds.count());

  while(true){
    std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
    std::chrono::microseconds useconds = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::system_clock::now().time_since_epoch());
    std::deque<chirp::Chirp> found_chirps = service_.monitor(request->username(), initial_time);
    initial_time.set_seconds(seconds.count());
    initial_time.set_useconds(useconds.count());
    for(chirp::Chirp c : found_chirps){
      chirp::MonitorReply reply;
      reply.set_allocated_chirp(&c);
      const chirp::MonitorReply& sendingReply = reply;
      stream->Write(sendingReply);
    }
    usleep(20);
  }
  return grpc::Status::OK;
}
