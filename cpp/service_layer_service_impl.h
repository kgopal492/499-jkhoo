#include <map>
#include <string>

#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"

// Class for logic and data behind the ServiceLayer
class ServiceLayerServiceImpl final : public chirp::ServiceLayer::Service {
 public:
  grpc::Status registeruser(grpc::ServerContext* context, const chirp::RegisterRequest* request, chirp::RegisterReply* reply) override;
  grpc::Status chirp(grpc::ServerContext* context, const chirp::ChirpRequest* request, chirp::ChirpReply* reply) override;
  grpc::Status follow(grpc::ServerContext* context, const chirp::FollowRequest* request, chirp::FollowReply* reply) override;
  grpc::Status read(grpc::ServerContext* context, const chirp::ReadRequest* request, chirp::ReadReply* reply) override;
  grpc::Status monitor(grpc::ServerContext* context, const chirp::MonitorRequest* request, chirp::MonitorReply* reply);

 private:
  enum ActionEnum {
    kuser_chirps = 0,
    kuser_following = 1,
    kchirp_text = 2,
    kchirp_replies = 3,
  };
  int curr_id_ = 0;
};
