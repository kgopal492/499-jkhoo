#ifndef CPP_SERVICE_LAYER_SERVICE_IMPL_H_
#define CPP_SERVICE_LAYER_SERVICE_IMPL_H_
#include <chrono>
#include <map>
#include <string>

#include <google/protobuf/util/time_util.h>
#include <grpcpp/grpcpp.h>
#include <unistd.h>
#include "key_value_client.h"
#include "service.grpc.pb.h"
#include "service.h"

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service
// from service.proto
class ServiceLayerServiceImpl final : public chirp::ServiceLayer::Service {
 public:
  // grpc function for registeruser
  grpc::Status registeruser(grpc::ServerContext* context,
                            const chirp::RegisterRequest* request,
                            chirp::RegisterReply* reply) override;
  // grpc function for chirp
  grpc::Status chirp(grpc::ServerContext* context,
                     const chirp::ChirpRequest* request,
                     chirp::ChirpReply* reply) override;
  // grpc function for follow
  grpc::Status follow(grpc::ServerContext* context,
                      const chirp::FollowRequest* request,
                      chirp::FollowReply* reply) override;
  // grpc function for read
  grpc::Status read(grpc::ServerContext* context,
                    const chirp::ReadRequest* request,
                    chirp::ReadReply* reply) override;
  // grpc function for monitor
  grpc::Status monitor(
      grpc::ServerContext* context, const chirp::MonitorRequest* request,
      grpc::ServerWriter< ::chirp::MonitorReply>* stream) override;

 private:
  // interface for grpc calls to key value store
  KeyValueClient clientInterface_;
  // keeps track of the id of the next chirp
  ServiceLayer service_ = ServiceLayer(&clientInterface_);
};
#endif
