#include "service.h"

void ServiceLayer::registeruser(const std::string& username) {
  if(!debug_mode_){
    chirp::PutRequest request1;
    const std::string& user_key = "0"+username;
    const std::string& empty = "";
    request1.set_key(user_key);
    request1.set_value(empty);
    chirp::PutReply reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->put(&context, request1, &reply);

    chirp::PutRequest request2;
    const std::string& following_user_key = "1"+username;
    request2.set_key(following_user_key);
    request2.set_value(empty);
    status = stub_->put(&context, request2, &reply);
  }
  else{
    const std::string& chirp_key = "0" + username;
    debug_store_.put(chirp_key, "");

    const std::string& chirp_key_following = "1" + username;
    debug_store_.put(chirp_key_following, "");
  }
}

chirp::Chirp ServiceLayer::chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
  // TODO: put the chirp into the key value store
  // get curr_id_ to determine what the ID will be of this chirp
  // PutRequest with key = ActionEnum.kChirpText + curr_id_ with value = list with text of chirp
  // PutRequest with key = ActionEnum.kChirpReplies + curr_id with value = null
  // add curr_id_ to key = ActionEnum.kUserChirps + username by doing a PutRequest
  // increment curr_id_
  chirp::Chirp placeholder;
  return placeholder;
}
void ServiceLayer::follow(const std::string& username, const std::string& to_follow) {
  // TODO: add to_follow to username's people following
  // PutRequest with key = ActionEnum.kUserFollowing + username with value = to_follow
}

chirp::Chirp ServiceLayer::read(const std::string& chirp_id) {
  // TODO: read chirp thread from given ID
  // GetRequest for text of given ID with key = ActionEnum.kChirpText + chirpId
  // recursively get replies with key = ActionEnum.kChirpReplies + chirpID and text for this thread until no more replies exist
  chirp::Chirp placeholder;
  return placeholder;
}

chirp::Chirp ServiceLayer::monitor(const std::string& username) {
  // TODO: stream chirps from following users
  // GetRequest for key = ActionEnum.kUserFollowing + username to get who is being followed
  // stream chirps
  chirp::Chirp placeholder;
  return placeholder;
}
