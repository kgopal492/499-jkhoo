#include "key_value_store_service_impl.h"

grpc::Status KeyValueStoreServiceImpl::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
  // TODO: put whatever the request says
  if (store_.find(request->key()) == store_.end()) {
    std::list<std::string> initialElement;
    initialElement.push_back(request->value());
    store_[request->key()] = initialElement;
  }
  else {
    store_[request->key()].push_back(request->value());
  }
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::get(grpc::ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) {
  // TODO: get whatever the request wants and set it as value in reply
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
  // TODO: delete what the request wants
  return grpc::Status::OK;
}
