#ifndef ADD_H
#define ADD_H
#include <gtest/gtest.h>

#include "../cpp/service_layer_service_impl.h"
#include "../cpp/service.h"
#include <google/protobuf/util/time_util.h>

// Registers a user and confirms that user key values are correct
TEST(RegisterUserTest, Simple) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);
  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());
}

// registers two users and makes one follow the other
TEST(RegisterUserTest, RegisterRegisterFollow) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());

  register_success = service_layer.registeruser("testUser2");
  ASSERT_EQ(true, register_success);

  const std::deque<std::string>& user_value_2 = test_store.get("0testUser2");
  ASSERT_EQ(1, user_value_2.size());

  const std::deque<std::string>& user_following_2 = test_store.get("1testUser2");
  ASSERT_EQ(1, user_following_2.size());

  bool follow_success = service_layer.follow("testUser", "testUser2");
  ASSERT_EQ(true, follow_success);
  const std::deque<std::string>& user_following_now = test_store.get("1testUser");
  ASSERT_EQ(2, user_following_now.size());
  ASSERT_EQ("testUser2", user_following_now.at(1));
}

// register a user, chirp from that user, and then read that chirp
TEST(RegisterUserTest, RegisterChirpRead) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());

  chirp::Chirp new_chirp = service_layer.chirp("testUser", "testText", "");
  const std::deque<std::string>& chirp_find = test_store.get("20");
  ASSERT_EQ(1, chirp_find.size());
  chirp::Chirp receivedChirp;
  receivedChirp.ParseFromString(chirp_find.at(0));
  ASSERT_EQ(new_chirp.text(), receivedChirp.text());
  ASSERT_EQ("testText", receivedChirp.text());

  std::deque<chirp::Chirp> read_chirps = service_layer.read("0");
  ASSERT_EQ(1, read_chirps.size());
  ASSERT_EQ("testText", read_chirps.at(0).text());
}

// register a user, chirp once, chirp again in response to that chirp, then read the thread
TEST(RegisterUserTest, RegisterChirpChirpRead) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());

  chirp::Chirp new_chirp = service_layer.chirp("testUser", "testText", "");
  chirp::Chirp new_chirp2 = service_layer.chirp("testUser", "testText2", "0");

  std::deque<chirp::Chirp> read_chirps = service_layer.read("0");
  ASSERT_EQ(2, read_chirps.size());
  ASSERT_EQ("testText", read_chirps.at(0).text());
  ASSERT_EQ("testText2", read_chirps.at(1).text());
}

// try to register an empty user
TEST(RegisterUserTest, RegisterEmptyUser) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("");
  ASSERT_EQ(false, register_success);
}

// register a user that already existing_user
TEST(RegisterUserTest, RegisterExistingUser) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("TestUser");
  ASSERT_EQ(true, register_success);
  register_success = service_layer.registeruser("TestUser");
  ASSERT_EQ(false, register_success);
}

// register two users, have the first follow the second, get a current timestamp, second user chirps twice, monitor starting from timestamp
TEST(FollowingUserTest, FollowMonitor) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);
  register_success = service_layer.registeruser("testUserToFollow");
  ASSERT_EQ(true, register_success);

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());
  const std::deque<std::string>& user_value2 = test_store.get("0testUserToFollow");
  ASSERT_EQ(1, user_value2.size());

  bool follow_success = service_layer.follow("testUser", "testUserToFollow");
  ASSERT_EQ(true, follow_success);
  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(2, user_following.size());

  chirp::Timestamp initial_time;
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetEpoch());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetEpoch());
  initial_time.set_seconds(seconds);
  initial_time.set_useconds(useconds);

  service_layer.chirp("testUserToFollow", "testText", "");
  service_layer.chirp("testUserToFollow", "testText2", "");
  std::deque<chirp::Chirp> monitored_chirps = service_layer.monitor("testUser", initial_time);
  ASSERT_EQ(2, monitored_chirps.size());
  ASSERT_EQ("testText", monitored_chirps.at(0).text());
  ASSERT_EQ("testText2", monitored_chirps.at(1).text());
}

// try to follow a user that does not exist
TEST(FollowingUserTest, FollowNonexistingUser) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);
  bool follow_success = service_layer.follow("testUser", "testUserToFollow");
  ASSERT_EQ(false, follow_success);
}

// chirp with invalid parent id
TEST(ChirpTest, InvalidParent) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  bool register_success = service_layer.registeruser("testUser");
  ASSERT_EQ(true, register_success);
  chirp::Chirp generated_chirp = service_layer.chirp("testUser", "hello world", "FAKE_ID");
  ASSERT_EQ("ERROR", generated_chirp.id());
}

// Tests whether hashtag and username added to stream are valid
TEST(StreamTest, CheckHashtagUser) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);

  const std::string kvalidUsername = "Krishna";
  const std::string kinvalidUsername = "Jill";
  const std::string kvalidHashtag = "499isgreat";
  const std::string kinvalidHashtag = "499 is great";
  chirp::Timestamp start_time;
  std::chrono::seconds seconds_since_start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time.set_seconds(seconds_since_start.count());
  start_time.set_useconds(useconds_since_start.count());

  // tests invalid hashtag returns false (tests beginstream)
  bool valid_hashtag = service_layer.beginstream(kinvalidHashtag, kvalidUsername, start_time);
  ASSERT_EQ(false, valid_hashtag);

  // tests invalid user returns false (tests beginstream)
  bool valid_user = service_layer.beginstream(kvalidHashtag, kinvalidUsername, start_time);
  ASSERT_EQ(false, valid_user);

  // tests valid hashtag & user returns true (tests beginstream)
  service_layer.registeruser(valid_user);
  bool valid_hashtag_user = service_layer.beginstream(kvalidHashtag, kinvalidUsername, start_time);
  ASSERT_EQ(true, valid_hashtag_user);
}

// Tests Stream from client end, ensuring that hashtags appropriately sent to single streamer
TEST(StreamTest, SingleUserValidHashtag) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);

  const std::string kvalidChirper = "Krishna";
  const std::string kvalidUsername = "Jill";
  const std::string klateStreamer = "Barath";
  const std::string kvalidHashtag = "499isgreat";
  const std::string kvalidChirpText = "Hey guys, I just wanted to let you know that #499isgreat";
  service_layer.registeruser(kvalidChirper);
  service_layer.registeruser(kvalidUsername);
  service_layer.registeruser(klateStreamer);
  chirp::Timestamp start_time;
  std::chrono::seconds seconds_since_start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time.set_seconds(seconds_since_start.count());
  start_time.set_useconds(useconds_since_start.count());

  // tests valid instance, hashtag chirped after user streams hashtags is sent to user
  service_layer.beginstream(kvalidHashtag, kinvalidUsername, start_time);
  service_layer.chirp(kvalidChirper, kvalidChirpText, "");
  std::deque<chirp::Chirp> chirp_results = service_layer.stream(kvalidHashtag, kvalidUsername, start_time);
  EXPECT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidChirpText, chirp_results[0].text();

  // tests hashtags chirped before stream begins are not added to stream
  service_layer.beginstream(kvalidHashtag, klateStreamer, start_time);
  std::deque<chirp::Chirp> late_chirp_results = service_layer.stream(kvalidHashtag, klateStreamer, start_time);
  ASSERT_EQ(0, late_chirp_results.size());
}

TEST(StreamTest, MultipleStreamHashtag) {
  // Multiple different users can stream same hashtag and should receive same chirps

  // The same user can stream the same hashtag separately and should receive same chirps
}

TEST(StreamTest, ValidHashtagParsing) {
  // hashtag correctly terminated by space in middle of string
  // hashtag starting at beginning of string correctly added
  // hashtag terminated by null character correctly added
  // hashtag length of zero should not be added
  // multiple different hashtags in chirp are all added to appropriate streamers
  // hashtag multiple times in chirp should only be printed once by streamer
}

TEST(StreamTest, EndStream) {
  // hashtag added after a user stops streaming is not listed when the user begins streaming once again
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
