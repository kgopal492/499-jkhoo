#ifndef CPP_KEY_VALUE_STORE_SERVICE_IMPL_H_
#define CPP_KEY_VALUE_STORE_SERVICE_IMPL_H_
#include <map>
#include <deque>
#include <string>
#include <mutex>

#include <grpcpp/grpcpp.h>
#include "key_value_store.h"
#include "backendStore.grpc.pb.h"

// Class for logic and data behind the KeyValueStore
class KeyValueStoreServiceImpl final : public chirp::KeyValueStore::Service {
 public:
  grpc::Status put(grpc::ServerContext* context, const chirp::PutRequest* request, chirp::PutReply* reply) override;
  grpc::Status get(grpc::ServerContext* context, grpc::ServerReaderWriter<chirp::GetReply, chirp::GetRequest>* stream);
  grpc::Status deletekey(grpc::ServerContext* context, const chirp::DeleteRequest* request, chirp::DeleteReply* reply) override;

 private:
  // Data structure behind the key value store
  KeyValueStore value_store_;
};
#endif
