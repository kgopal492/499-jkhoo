#include "key_value_store_service_impl.h"

// Listens on port 50000 and assembles the server
void RunServer() {
  // TODO: listen on port 50000
  std::string server_address("0.0.0.0:50000");
  KeyValueStoreServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
  // TODO: register the KeyValueStoreService
  // TODO: wait
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
