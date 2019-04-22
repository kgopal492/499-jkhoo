#include "service.h"

ServiceLayer::ServiceLayer(KeyValueClientInterface* key_value_connection) {
  std::lock_guard<std::mutex> lock(service_mut_);
  store_ = key_value_connection;
  const std::deque<std::string>& chirp_count = store_->get("chirp_count");
  if (chirp_count.size() == 0) {
    store_->put("chirp_count", "0");
  }
}
bool ServiceLayer::registeruser(const std::string& username) {
  std::lock_guard<std::mutex> lock(service_mut_);
  if (username.length() == 0) {
    return false;
  }
  const std::string userKey = kuserChirps_ + username;
  const std::deque<std::string>& this_user = store_->get(userKey);
  if (this_user.size() == 0) {
    const std::string empty = "";
    store_->put(userKey, empty);
    const std::string followingUserKey = kuserFollowing_ + username;
    store_->put(followingUserKey, empty);
    return true;
  }
  return false;
}

chirp::Chirp ServiceLayer::chirpConstructionHelper(
    const std::string& username, const std::string& text,
    const std::string& parent_id) {
  std::string my_id;
  const std::deque<std::string>& chirp_count = store_->get("chirp_count");
  if (chirp_count.size() == 0) {
    chirp::Chirp error_chirp;
    error_chirp.set_id("ERROR");
    return error_chirp;
  }
  my_id = chirp_count.at(0);
  int curr_id_ = std::stoi(my_id) + 1;
  store_->deletekey("chirp_count");
  store_->put("chirp_count", std::to_string(curr_id_));
  chirp::Chirp this_chirp;
  this_chirp.set_username(username);
  this_chirp.set_text(text);
  this_chirp.set_id(my_id);
  this_chirp.set_parent_id(parent_id);
  chirp::Timestamp* chirp_timestamp = new chirp::Timestamp();
  std::chrono::seconds seconds =
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  chirp_timestamp->set_seconds(seconds.count());
  chirp_timestamp->set_useconds(useconds.count());
  this_chirp.set_allocated_timestamp(chirp_timestamp);
  return this_chirp;
}

chirp::Chirp ServiceLayer::chirp(const std::string& username,
                                 const std::string& text,
                                 const std::string& parent_id) {
  std::lock_guard<std::mutex> lock(service_mut_);
  if (parent_id.length() > 0) {
    const std::string this_chirp_parent_key = kchirpValue_ + parent_id;
    const std::deque<std::string>& this_chirps_values =
        store_->get(this_chirp_parent_key);
    if (this_chirps_values.size() == 0) {
      chirp::Chirp error_chirp;
      error_chirp.set_id("ERROR");
      return error_chirp;
    }
  }
  const std::string this_user_key = kuserChirps_ + username;
  const std::deque<std::string>& this_user_info = store_->get(this_user_key);
  if (this_user_info.size() == 0) {
    chirp::Chirp error_chirp;
    error_chirp.set_id("ERROR");
    return error_chirp;
  }

  chirp::Chirp this_chirp = chirpConstructionHelper(username, text, parent_id);
  std::string my_id = this_chirp.id();
  if (my_id == "ERROR") {
    return this_chirp;
  }

  const std::string this_chirp_key = kchirpValue_ + my_id;
  std::string chirp_as_string;
  this_chirp.SerializeToString(&chirp_as_string);
  const std::string value = chirp_as_string;
  store_->put(this_chirp_key, value);

  const std::string user_key = kuserChirps_ + username;
  store_->put(user_key, my_id);

  const std::string this_chirp_reply_key = kchirpReplies_ + my_id;
  const std::string empty = "";
  store_->put(this_chirp_reply_key, empty);

  if (parent_id.length() > 0) {
    const std::string chirp_parent_id = kchirpReplies_ + parent_id;
    const std::string this_chirp_id = my_id;
    store_->put(chirp_parent_id, my_id);
  }

  storeHashtags(text, my_id);

  return this_chirp;
}

void ServiceLayer::storeHashtags(const std::string& text, const std::string& chirp_id) {
  // TODO: check whether hashtag contains '#' character
  std::string temp_text = text;
  size_t hash_index = temp_text.find("#");
  while (hash_index != std::string::npos) {
    std::string hashtag = temp_text.substr(hash_index+1);
    // TODO: check whether hash_index+1 is a position that exists
    size_t space_index = hashtag.find(" ");
    if (space_index  != std::string::npos) {
      hashtag = hashtag.substr(0, space_index);
      temp_text = temp_text.substr(space_index);
    } else {
      temp_text = temp_text.substr(temp_text.length());
    }
    if (hashtag.length() != 0) {
      const std::string hashtag_key = khashtag_ + hashtag;
      const std::deque<std::string>& hashtag_chirp_ids = store_->get(hashtag_key);
      bool chirp_added = false;
      for (std::string curr_hash_chirp_id : hashtag_chirp_ids) {
        if(curr_hash_chirp_id == chirp_id) {
          chirp_added = true;
          break;
        }
      }
      if (!chirp_added) {
        std::cout << "putting hashtag in kvs: " << hashtag << std:: endl;
        store_->put(hashtag_key, chirp_id);
        std::deque<std::string> chirp_ids = store_->get(hashtag_key);
        for(std::string id : chirp_ids) {
          std::cout << "Curr chirp id: " << id << std::endl;
        }
      }
    }
    std::cout << "HASHTAG: #" << hashtag << std:: endl;
    hash_index = temp_text.find("#");
  }
}

bool ServiceLayer::follow(const std::string& username,
                          const std::string& to_follow) {
  std::lock_guard<std::mutex> lock(service_mut_);
  if (username == to_follow) {
    return false;
  }
  const std::string to_follow_user = kuserChirps_ + to_follow;
  const std::deque<std::string>& follow_users_values =
      store_->get(to_follow_user);
  if (follow_users_values.size() == 0) {
    return false;
  }
  const std::string this_user_key = kuserChirps_ + username;
  const std::deque<std::string>& this_user_info = store_->get(this_user_key);
  if (this_user_info.size() == 0) {
    return false;
  }
  const std::string following_user_key = kuserFollowing_ + username;
  store_->put(following_user_key, to_follow);
  return true;
}

std::deque<chirp::Chirp> ServiceLayer::read(const std::string& chirp_id) {
  std::lock_guard<std::mutex> lock(service_mut_);
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
      const std::string this_chirp_key = kchirpValue_ + curr_chirp_id;
      const std::deque<std::string>& this_chirps_values =
          store_->get(this_chirp_key);
      thisChirp.ParseFromString(this_chirps_values.at(0));
      read_chirps.push_back(thisChirp);

      // Adds ids of thisChirp's replies to chirp_list to be read
      const std::string this_chirp_reply_key = kchirpReplies_ + curr_chirp_id;
      const std::deque<std::string>& this_chirp_replies =
          store_->get(this_chirp_reply_key);
      for (const std::string reply : this_chirp_replies) {
        chirp_list.push_back(reply);
      }
    }
  }
  std::sort(read_chirps.begin(), read_chirps.end(),
            [](const chirp::Chirp& chirp1, const chirp::Chirp& chirp2) {
              return chirp1.timestamp().useconds() <
                     chirp2.timestamp().useconds();
            });

  return read_chirps;
}

std::deque<chirp::Chirp> ServiceLayer::monitor(const std::string& username,
                                               chirp::Timestamp start) {
  std::lock_guard<std::mutex> lock(service_mut_);
  std::deque<chirp::Chirp> found_chirps;
  const std::string user_following_key = kuserFollowing_ + username;
  const std::deque<std::string>& user_following =
      store_->get(user_following_key);
  for (std::string username : user_following) {
    if (username.length() > 0) {
      const std::deque<std::string>& user_chirp_ids =
          store_->get(kuserChirps_ + username);
      for (const std::string& id : user_chirp_ids) {
        std::deque<std::string> this_chirps_values =
            store_->get(kchirpValue_ + id);
        chirp::Chirp thisChirp;
        if (this_chirps_values.size() > 0) {
          thisChirp.ParseFromString(this_chirps_values.at(0));
          int64_t myMicroSeconds = thisChirp.timestamp().useconds();
          int64_t baslineSeconds = start.useconds();
          if (thisChirp.timestamp().useconds() > start.useconds()) {
            found_chirps.push_back(thisChirp);
          }
        }
      }
    }
  }

  return found_chirps;
}

std::deque<chirp::Chirp> ServiceLayer::stream(const std::string& hashtag) {
  std::lock_guard<std::mutex> lock(service_mut_);
  // get initial_time in which hashtags are requested
  chirp::Timestamp start;
  std::chrono::seconds current_time_seconds =
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds current_time_useconds =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  start.set_seconds(current_time_seconds.count());
  start.set_useconds(current_time_useconds.count());
  // TODO: check if username exists
  std::deque<chirp::Chirp> found_chirps;
  const std::string hashtag_key = khashtag_ + hashtag;
  const std::deque<std::string>& hashtag_chirps =
      store_->get(hashtag_key);
  for (const std::string& id : hashtag_chirps) {
    std::deque<std::string> this_chirps_values =
        store_->get(kchirpValue_ + id);
    chirp::Chirp thisChirp;
    if (this_chirps_values.size() > 0) {
      thisChirp.ParseFromString(this_chirps_values.at(0));
      if (thisChirp.timestamp().useconds() > start.useconds()) {
        found_chirps.push_back(thisChirp);
      }
    }
  }

  return found_chirps;
}
