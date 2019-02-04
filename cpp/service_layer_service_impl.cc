#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  service_.registeruser(request->username());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  chirp::Chirp this_chirp = service_.chirp(request->username(), request->text(), request->parent_id());
  reply->set_allocated_chirp(&this_chirp);
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
  chirp::Chirp* chirp_pointer = reply->add_chirps();

  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply) {
  // TODO: stream chirps from following users
  // GetRequest for key = ActionEnum.kUserFollowing + username to get who is being followed
  // stream chirps
  return grpc::Status::OK;
}
