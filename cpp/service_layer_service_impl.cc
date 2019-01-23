#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) override {
  // TODO: register the user in the key value store
  // PutRequest with key = ActionEnum.kuser_chirps + username with value = null
  // PutRequest with key = ActionEnum.kuser_following + username with value = null
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) override {
  // TODO: put the chirp into the key value store
  // get curr_id_ to determine what the ID will be of this chirp
  // PutRequest with key = ActionEnum.kchirp_text + curr_id_ with value = list with text of chirp
  // PutRequest with key = ActionEnum.kchirp_replies + curr_id with value = null
  // add curr_id_ to key = ActionEnum.kuser_chirps + username by doing a PutRequest
  // increment curr_id_
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServiceImpl::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) override {
  // TODO: add to_follow to username's people following
  // PutRequest with key = ActionEnum.kuser_following + username with value = to_follow
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) override {
  // TODO: read chirp thread from given ID
  // GetRequest for text of given ID with key = ActionEnum.kchirp_text + chirpId
  // recursively get replies with key = ActionEnum.kchirp_replies + chirpID and text for this thread until no more replies exist
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply) {
  // TODO: stream chirps from following users
  // GetRequest for key = ActionEnum.kuser_following + username to get who is being followed
  // stream chirps
  return grpc::Status::OK;
}
