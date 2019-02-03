#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) {
  service_.registeruser(request->username());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) {
  // TODO: put the chirp into the key value store
  // get curr_id_ to determine what the ID will be of this chirp
  // PutRequest with key = ActionEnum.kChirpText + curr_id_ with value = list with text of chirp
  // PutRequest with key = ActionEnum.kChirpReplies + curr_id with value = null
  // add curr_id_ to key = ActionEnum.kUserChirps + username by doing a PutRequest
  // increment curr_id_
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServiceImpl::follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) {
  service_.follow(request->username(), request->to_follow());
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) {
  // TODO: read chirp thread from given ID
  // GetRequest for text of given ID with key = ActionEnum.kChirpText + chirpId
  // recursively get replies with key = ActionEnum.kChirpReplies + chirpID and text for this thread until no more replies exist
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply) {
  // TODO: stream chirps from following users
  // GetRequest for key = ActionEnum.kUserFollowing + username to get who is being followed
  // stream chirps
  return grpc::Status::OK;
}
