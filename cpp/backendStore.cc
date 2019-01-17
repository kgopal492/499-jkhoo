#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"

class KeyValueStoreServiceImpl final : public chirp::KeyValueStore::Service{
  grpc::Status put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override {
    // put whatever the request says
    return grpc::Status::OK;
  }
  grpc::Status get(grpc::ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply) {
    // get whatever the request wants and set it as value in reply
    return grpc::Status::OK;
  }
  grpc::Status deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) override {
    // delete what the request wants
    return grpc::Status::OK;
  }
};

void RunServer(){
  // listen on port 50000
  // register the KeyValueStoreService
  // wait
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
