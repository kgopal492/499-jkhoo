#include <map>
#include <list>
#include <string>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include "backendStore.grpc.pb.h"

// Class for logic and data behind the KeyValueStore
class KeyValueStoreServiceImpl final : public chirp::KeyValueStore::Service {
 public:
  grpc::Status put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override;
  grpc::Status get(grpc::ServerContext* context, const chirp::GetRequest* request, chirp::GetReply* reply);
  grpc::Status deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) override;

 private:
  // Map to store the keys and values of the KeyValueStoreServiceImpl
  std::map<std::string, std::list<std::string>> store_;
  std::mutex store_mut_;
};
