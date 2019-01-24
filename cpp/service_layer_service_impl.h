#include <map>
#include <string>

#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"
#include "service.h"

// Class for logic and data behind the ServiceLayer
// The methods in this class are the implementation of the Service Layer Service from service.proto
class ServiceLayerServiceImpl final : public chirp::ServiceLayer::Service {
 public:
  grpc::Status registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) override;
  grpc::Status chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) override;
  grpc::Status follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) override;
  grpc::Status read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) override;
  grpc::Status monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply);

 private:
  // keeps track of the id of the next chirp
  ServiceLayer service_;
};
