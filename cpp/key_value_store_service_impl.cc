#include "key_value_store_service_impl.h"


grpc::Status KeyValueStoreServiceImpl::put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) {
  value_store_.put(request->key(), request->value());
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream) {
  chirp::GetRequest request;
  stream->Read(&request);
  const std::deque<std::string>& values = value_store_.get(request.key());

  chirp::GetReply reply;
  for (const std::string& val : values) {
    reply.set_value(val);
    const chirp::GetReply& sendingReply = reply;
    stream->Write(sendingReply);
  }

  return grpc::Status::OK;
}

grpc::Status KeyValueStoreServiceImpl::deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) {
  value_store_.deletekey(request->key());
  return grpc::Status::OK;
}
