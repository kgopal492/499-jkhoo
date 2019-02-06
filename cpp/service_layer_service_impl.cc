#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  std::cout<<"In register user"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  service_.registeruser(request->username());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  std::cout<<"In chirp"<<std::endl;
  std::cout<<request->DebugString()<<std::endl;
  chirp::Chirp this_chirp = service_.chirp(request->username(), request->text(), request->parent_id());
  chirp::Chirp* reply_chirp = reply->mutable_chirp();
  this_chirp.set_username("fake_username");
  this_chirp.set_text("fake_text");
  //reply->set_allocated_chirp(&this_chirp);
  //reply_chirp->CopyFrom(this_chirp);
  *reply_chirp = this_chirp;
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServiceImpl::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) {
  service_.follow(request->username(), request->to_follow());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) {
  std::deque<chirp::Chirp> read_chirps = service_.read(request->chirp_id());
  for(chirp::Chirp c : read_chirps){
    chirp::Chirp* chirp_pointer = reply->add_chirps();
    const chirp::Chirp& added_chirp = c;
    chirp_pointer->CopyFrom(added_chirp);
  }
  //chirp::Chirp* chirp_pointer = reply->add_chirps();

  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::MonitorReply, chirp::MonitorRequest>* stream) {
  chirp::MonitorRequest request;
  stream->Read(&request);

  chirp::Timestamp initial_time;
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetEpoch());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetEpoch());
  initial_time.set_seconds(seconds);
  initial_time.set_useconds(useconds);

  while(true){
    seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetEpoch());
    useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetEpoch());
    std::deque<chirp::Chirp> found_chirps = service_.monitor(request.username(), initial_time);
    for(chirp::Chirp c : found_chirps){
      chirp::MonitorReply reply;
      reply.set_allocated_chirp(&c);
      const chirp::MonitorReply& sendingReply = reply;
      stream->Write(sendingReply);
    }
    initial_time.set_seconds(seconds);
    initial_time.set_useconds(useconds);
    usleep(10);
  }
  return grpc::Status::OK;
}
