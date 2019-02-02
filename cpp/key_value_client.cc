#include "key_value_client.h"


void KeyValueClient::put(const std::string& key, const std::string& value) {
  chirp::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  chirp::PutReply reply;
  grpc::ClientContext context;
  grpc::Status status = stub_->put(&context, request, &reply);
}

const std::deque<std::string>& KeyValueClient::get(const std::string& key) {
  chirp::GetRequest request;
  request.set_key(key);
  chirp::GetReply reply;
  grpc::ClientContext context;
  // TODO: For some odd reason the next line does not work, figure out why later
  grpc::Status status = stub_->get(&context, &request, &reply);
  // TODO: actuall get values and put into deque
  std::deque<std::string> placeholder;
  placeholder.push_back("fake info");
  const std::deque<std::string>& placeholderReturnValue = placeholder;
  return placeholderReturnValue;
}

void KeyValueClient::deletekey(const std::string& key) {
  chirp::DeleteRequest request;
  request.set_key(key);
  chirp::DeleteReply reply;
  grpc::ClientContext context;
  grpc::Status status = stub_->deletekey(&context, request, &reply);
}
