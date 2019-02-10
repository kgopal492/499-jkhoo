#include "service.h"

ServiceLayer::ServiceLayer(KeyValueClientInterface* key_value_connection) {
  store_ = key_value_connection;
  id_mut_ = new std::mutex();
}
bool ServiceLayer::registeruser(const std::string& username) {
  const std::string userKey = kuserChirps_+username;
  const std::deque<std::string>& this_user = store_->get(userKey);
  if(this_user.size() == 0){
    const std::string empty = "";
    store_->put(userKey, empty);
    const std::string followingUserKey = kuserFollowing_+username;
    store_->put(followingUserKey, empty);
    return true;
  }
  return false;
}

chirp::Chirp ServiceLayer::chirp(const std::string& username, const std::string& text, const std::string& parent_id) {
  std::string my_id;
  {
    std::lock_guard<std::mutex> lock(*id_mut_);
    my_id = std::to_string(curr_id_);
    curr_id_ = curr_id_ + 1;
  }
  chirp::Chirp this_chirp;
  this_chirp.set_username(username);
  this_chirp.set_text(text);
  this_chirp.set_id(my_id);
  this_chirp.set_parent_id(parent_id);
  chirp::Timestamp* chirp_timestamp = new chirp::Timestamp();
  std::chrono::seconds seconds = std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds = std::chrono::duration_cast< std::chrono::microseconds >(std::chrono::system_clock::now().time_since_epoch());
  chirp_timestamp->set_seconds(seconds.count());
  chirp_timestamp->set_useconds(useconds.count());
  this_chirp.set_allocated_timestamp(chirp_timestamp);

  const std::string this_chirp_key = kchirpValue_ + my_id;
  std::string chirp_as_string;
  this_chirp.SerializeToString(&chirp_as_string);
  const std::string value = chirp_as_string;
  store_->put(this_chirp_key, value);

  const std::string user_key = kuserChirps_+username;
  store_->put(user_key, my_id);

  const std::string this_chirp_reply_key = kchirpReplies_+my_id;
  const std::string empty = "";
  store_->put(this_chirp_reply_key, empty);

  if (parent_id.length() > 0) {
    const std::string chirp_parent_id = kchirpReplies_+parent_id;
    const std::string this_chirp_id = my_id;
    store_->put(chirp_parent_id, my_id);
  }
  return this_chirp;
}
void ServiceLayer::follow(const std::string& username, const std::string& to_follow) {
  const std::string following_user_key = kuserFollowing_+username;
  store_->put(following_user_key, to_follow);
}

std::deque<chirp::Chirp> ServiceLayer::read(const std::string& chirp_id) {
  std::deque<chirp::Chirp> read_chirps;

  // Stores chirps to read
  std::deque<std::string> chirp_list;
  std::string given_chirp = chirp_id;
  chirp_list.push_back(given_chirp);

  while (chirp_list.size() > 0) {
    const std::string curr_chirp_id = chirp_list.at(0);
    chirp_list.pop_front();

    if (curr_chirp_id.length() > 0) {
      // Adds chirps to the deque of chirps in this thread
      chirp::Chirp thisChirp;
      const std::string this_chirp_key = kchirpValue_+curr_chirp_id;
      const std::deque<std::string>& this_chirps_values = store_->get(this_chirp_key);
      thisChirp.ParseFromString(this_chirps_values.at(0));
      read_chirps.push_back(thisChirp);

      // Adds ids of thisChirp's replies to chirp_list to be read
      const std::string this_chirp_reply_key = kchirpReplies_+curr_chirp_id;
      const std::deque<std::string>& this_chirp_replies = store_->get(this_chirp_reply_key);
      for (const std::string reply : this_chirp_replies) {
        chirp_list.push_back(reply);
      }
    }
  }

  return read_chirps;
}

std::deque<chirp::Chirp> ServiceLayer::monitor(const std::string& username, chirp::Timestamp start) {
  std::deque<chirp::Chirp> found_chirps;
  const std::string user_following_key = kuserFollowing_ + username;
  const std::deque<std::string>& user_following = store_->get(user_following_key);
  for(std::string username : user_following){
    if(username.length() > 0){
      const std::deque<std::string>& user_chirp_ids = store_->get(kuserChirps_ + username);
      for(const std::string& id : user_chirp_ids){
        std::deque<std::string> this_chirps_values = store_->get(kchirpValue_ + id);
        chirp::Chirp thisChirp;
        if(this_chirps_values.size() > 0){
          thisChirp.ParseFromString(this_chirps_values.at(0));
          int64_t myMicroSeconds = thisChirp.timestamp().useconds();
          int64_t baslineSeconds = start.useconds();
          std::string thisText = thisChirp.text();
          if(thisChirp.timestamp().useconds() > start.useconds()){
            found_chirps.push_back(thisChirp);
            std::cout<<thisChirp.text()<<std::endl;
          }
        }
      }
    }
  }

  return found_chirps;
}
