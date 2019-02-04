#include "service.h"

ServiceLayer::ServiceLayer(KeyValueClientInterface* key_value_connection) {
  store_ = key_value_connection;
}
void ServiceLayer::registeruser(const std::string& username) {
  const std::string& user_key = kuser_chirps_+username;
  const std::string& empty = "";
  store_->put(user_key, empty);

  const std::string& following_user_key = kuser_following_+username;
  store_->put(following_user_key, empty);
}

chirp::Chirp ServiceLayer::chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
  const std::string& my_id = std::to_string(curr_id_);
  curr_id_ = curr_id_ + 1;
  chirp::Chirp this_chirp;
  this_chirp.set_username(username);
  this_chirp.set_text(text);
  this_chirp.set_id(my_id);
  this_chirp.set_parent_id(parent_id);
  chirp::Timestamp* chirp_timestamp = new chirp::Timestamp();
  chirp_timestamp->set_seconds(time(NULL));
  this_chirp.set_allocated_timestamp(chirp_timestamp);

  const std::string& this_chirp_key = kchirp_value_ + my_id;
  std::string chirp_as_string;
  this_chirp.SerializeToString(&chirp_as_string);
  const std::string& value = chirp_as_string;
  store_->put(this_chirp_key, value);

  const std::string& user_key = kuser_chirps_+username;
  store_->put(user_key, my_id);

  const std::string& this_chirp_reply_key = kchirp_replies_+my_id;
  const std::string& empty = "";
  store_->put(this_chirp_reply_key, empty);

  if (parent_id.length() > 0) {
    const std::string& chirp_parent_id = kchirp_replies_+parent_id;
    const std::string& this_chirp_id = my_id;
    store_->put(chirp_parent_id, my_id);
  }
  return this_chirp;
}
void ServiceLayer::follow(const std::string& username, const std::string& to_follow) {
  const std::string& following_user_key = kuser_following_+username;
  store_->put(following_user_key, to_follow);
}

std::deque<chirp::Chirp> ServiceLayer::read(const std::string& chirp_id) {
  std::deque<chirp::Chirp> read_chirps;

  // Stores chirps to read
  std::deque<std::string> chirp_list;
  std::string given_chirp = chirp_id;
  chirp_list.push_back(given_chirp);

  while (chirp_list.size() > 0) {
    const std::string& curr_chirp_id = chirp_list.at(0);
    chirp_list.pop_front();

    if (curr_chirp_id.length() > 0) {
      // Adds chirps to the deque of chirps in this thread
      chirp::Chirp thisChirp;
      const std::string& this_chirp_key = kchirp_value_+curr_chirp_id;
      const std::deque<std::string>& this_chirps_values = store_->get(this_chirp_key);
      thisChirp.ParseFromString(this_chirps_values.at(0));
      read_chirps.push_back(thisChirp);

      // Adds ids of thisChirp's replies to chirp_list to be read
      const std::string& this_chirp_reply_key = kchirp_replies_+curr_chirp_id;
      const std::deque<std::string>& this_chirp_replies = store_->get(this_chirp_reply_key);
      for (const std::string& reply : this_chirp_replies) {
        chirp_list.push_back(reply);
      }
    }
  }

  return read_chirps;
}

chirp::Chirp ServiceLayer::monitor(const std::string& username) {
  // TODO: stream chirps from following users
  // GetRequest for key = ActionEnum.kUserFollowing + username to get who is being followed
  // stream chirps
  chirp::Chirp placeholder;
  return placeholder;
}
