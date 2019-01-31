#include "service.h"

ServiceLayer::ServiceLayer(KeyValueClientInterface* key_value_connection) {
  store_ = key_value_connection;
}
void ServiceLayer::registeruser(const std::string& username) {
  const std::string& user_key = "0"+username;
  const std::string& empty = "";
  store_->put(user_key, empty);

  const std::string& following_user_key = "1"+username;
  store_->put(following_user_key, empty);
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
