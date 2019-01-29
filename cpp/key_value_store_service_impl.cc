#include "key_value_store_service_impl.h"


grpc::Status KeyValueStoreServiceImpl::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
  value_store_.put(request->key(), request->value());
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::get(grpc::ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) {
  const std::deque<std::string>& values = value_store_.get(request->key());
  chirp::StoreValues returnVals;
  for(const std::string& val: values){
    returnVals.add_values(val);
  }
  std::string finalVal;
  returnVals.SerializeToString(&finalVal);
  reply->set_value(finalVal);
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
  value_store_.deletekey(request->key());
  return grpc::Status::OK;
}
