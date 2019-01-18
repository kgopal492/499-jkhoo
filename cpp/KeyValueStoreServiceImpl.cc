#include "KeyValueStoreServiceImpl.h"

grpc::Status KeyValueStoreServiceImpl::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override {
  // TODO: put whatever the request says
  return grpc::Status::OK;
} 

grpc::Status KeyValueStoreServiceImpl::get(grpc::ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) {
  // TODO: get whatever the request wants and set it as value in reply
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) override {
  // TODO: delete what the request wants
  return grpc::Status::OK;
}
