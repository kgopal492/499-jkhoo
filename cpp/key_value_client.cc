#include "key_value_client.h"


void KeyValueClient::put(const std::string& key, const std::string& value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  grpc::ClientContext context;
  grpc::Status status = stub_->put(&context, request, &reply);
}

std::deque<std::string> KeyValueClient::get(const std::string& key) {
  chirp::GetRequest request;
  request.set_key(key);
  chirp::GetReply reply;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientReaderWriter<chirp::GetRequest, chirp::GetReply> > stream_handle (stub_->get(&context));
  stream_handle->Write(request);

  std::deque<std::string> returnValues;
  while(stream_handle->Read(&reply)){
    returnValues.push_back(reply.value());
  }
  std::cout<<returnValues.at(0)<<std::endl;

  return returnValues;
}

void KeyValueClient::deletekey(const std::string& key) {
  chirp::DeleteRequest request;
  request.set_key(key);
  chirp::DeleteReply reply;
  grpc::ClientContext context;
  grpc::Status status = stub_->deletekey(&context, request, &reply);
}
