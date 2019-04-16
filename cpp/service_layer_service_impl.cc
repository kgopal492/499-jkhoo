#include "service_layer_service_impl.h"

grpc::Status ServiceLayerServiceImpl::registeruser(
    grpc::ServerContext* context, const chirp::RegisterRequest* request,
    chirp::RegisterReply* reply) {
  std::string username = request->username();
  bool response = service_.registeruser(username);
  if (!response) {
    grpc::Status existing_user = grpc::Status(
        grpc::StatusCode::INVALID_ARGUMENT, "Usernames must be unique");
    return existing_user;
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::chirp(grpc::ServerContext* context,
                                            const chirp::ChirpRequest* request,
                                            chirp::ChirpReply* reply) {
  chirp::Chirp this_chirp = service_.chirp(request->username(), request->text(),
                                           request->parent_id());
  if (this_chirp.id() == "ERROR") {
    grpc::Status parent_not_found =
        grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid Parent ID");
    return parent_not_found;
  }
  chirp::Chirp* reply_chirp = reply->mutable_chirp();
  *reply_chirp = this_chirp;
  return grpc::Status::OK;
}
grpc::Status ServiceLayerServiceImpl::follow(
    grpc::ServerContext* context, const chirp::FollowRequest* request,
    chirp::FollowReply* reply) {
  bool success = service_.follow(request->username(), request->to_follow());
  if (!success) {
    grpc::Status follow_failed =
        grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Follow failed");
    return follow_failed;
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::read(grpc::ServerContext* context,
                                           const chirp::ReadRequest* request,
                                           chirp::ReadReply* reply) {
  std::deque<chirp::Chirp> read_chirps = service_.read(request->chirp_id());
  for (chirp::Chirp c : read_chirps) {
    chirp::Chirp* chirp_pointer = reply->add_chirps();
    const chirp::Chirp& added_chirp = c;
    chirp_pointer->CopyFrom(added_chirp);
  }
  chirp::Chirp* chirp_pointer = reply->add_chirps();

  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::monitor(
    grpc::ServerContext* context, const chirp::MonitorRequest* request,
    grpc::ServerWriter< ::chirp::MonitorReply>* stream) {
  chirp::Timestamp initial_time;
  std::chrono::seconds seconds =
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  initial_time.set_seconds(seconds.count());
  initial_time.set_useconds(useconds.count());
  std::set<std::string> read_chirps;
  bool keep_monitoring = true;
  std::deque<chirp::Chirp> found_chirps;
  while (keep_monitoring) {
    seconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch());
    useconds = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    found_chirps = service_.monitor(request->username(), initial_time);
    initial_time.set_seconds(seconds.count());
    initial_time.set_useconds(useconds.count());
    for (chirp::Chirp c : found_chirps) {
      if (read_chirps.find(c.id()) == read_chirps.end()) {
        chirp::Chirp* this_chirp = new chirp::Chirp();
        this_chirp->CopyFrom(c);
        chirp::MonitorReply reply;
        reply.set_allocated_chirp(this_chirp);
        chirp::MonitorReply sendingReply = reply;
        stream->Write(sendingReply);
        read_chirps.insert(c.id());
      }
    }
    if (context->IsCancelled()) {
      keep_monitoring = false;
    }
    usleep(20);
  }
  return grpc::Status::OK;
}

grpc::Status ServiceLayerServiceImpl::stream(
    grpc::ServerContext* context, const chirp::StreamRequest* request,
    grpc::ServerWriter< ::chirp::StreamReply>* stream) {
  chirp::Timestamp initial_time;
  std::chrono::seconds current_time_seconds =
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds current_time_useconds =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  initial_time.set_seconds(current_time_seconds.count());
  initial_time.set_useconds(current_time_useconds.count());
  std::set<std::string> read_chirps;
  bool keep_streaming = true;
  std::deque<chirp::Chirp> found_chirps;
  while (keep_streaming) {
    current_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch());
    current_time_useconds = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    found_chirps = service_.stream(request->hashtag(), initial_time);
    initial_time.set_seconds(current_time_seconds.count());
    initial_time.set_useconds(current_time_useconds.count());
    for (const chirp::Chirp c : found_chirps) {
      if (read_chirps.find(c.id()) == read_chirps.end()) {
        chirp::Chirp* this_chirp = new chirp::Chirp();
        this_chirp->CopyFrom(c);
        chirp::StreamReply reply;
        reply.set_allocated_chirp(this_chirp);
        chirp::StreamReply sendingReply = reply;
        stream->Write(sendingReply);
        read_chirps.insert(c.id());
      }
    }
    if (context->IsCancelled()) {
      keep_streaming = false;
    }
  }
  return grpc::Status::OK;
}
