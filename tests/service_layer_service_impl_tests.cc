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
  service_layer.registeruser(kvalidUsername);
  bool valid_hashtag_user = service_layer.beginstream(kvalidHashtag, kvalidUsername, start_time);
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
  service_layer.beginstream(kvalidHashtag, kvalidUsername, start_time);
  service_layer.chirp(kvalidChirper, kvalidChirpText, "");
  std::deque<chirp::Chirp> chirp_results = service_layer.stream(kvalidHashtag, kvalidUsername, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidChirpText, chirp_results[0].text());

  // tests hashtags chirped before stream begins are not added to stream
  service_layer.beginstream(kvalidHashtag, klateStreamer, start_time);
  std::deque<chirp::Chirp> late_chirp_results = service_layer.stream(kvalidHashtag, klateStreamer, start_time);
  ASSERT_EQ(0, late_chirp_results.size());
}

// Tests whether multiple different users can stream the same hashtag simultaneously
TEST(StreamTest, MultipleStreamHashtag) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  const std::string kchirper = "Krishna";
  const std::string kmultipleUser1 = "Jill";
  const std::string kmultipleUser2 = "Barath";
  const std::string kvalidHashtag = "499isgreat";
  const std::string kvalidChirpText = "Hey guys, I just wanted to let you know that #499isgreat";
  service_layer.registeruser(kchirper);
  service_layer.registeruser(kmultipleUser1);
  service_layer.registeruser(kmultipleUser2);
  chirp::Timestamp start_time;
  std::chrono::seconds seconds_since_start1 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time.set_seconds(seconds_since_start1.count());
  start_time.set_useconds(useconds_since_start1.count());
  service_layer.beginstream(kvalidHashtag, kmultipleUser1, start_time);
  service_layer.beginstream(kvalidHashtag, kmultipleUser2, start_time);
  service_layer.chirp(kchirper, kvalidChirpText, "");
  std::deque<chirp::Chirp> chirp_results1 = service_layer.stream(kvalidHashtag, kmultipleUser1, start_time);
  ASSERT_EQ(1, chirp_results1.size());
  ASSERT_EQ(kvalidChirpText, chirp_results1[0].text());
  std::deque<chirp::Chirp> chirp_results2 = service_layer.stream(kvalidHashtag, kmultipleUser2, start_time);
  ASSERT_EQ(1, chirp_results2.size());
  ASSERT_EQ(kvalidChirpText, chirp_results2[0].text());
}

// Tests whether multiple instances of the same user can stream the same hashtag simultaneously
TEST(StreamTest, SameUserStreamTwice) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  const std::string ksameUser = "Krishna";
  const std::string kchirper = "Jill";
  const std::string kvalidHashtag = "499isgreat";
  const std::string kvalidChirpText = "Hey guys, I just wanted to let you know that #499isgreat";
  service_layer.registeruser(ksameUser);
  service_layer.registeruser(kchirper);
  chirp::Timestamp start_time_user1;
  std::chrono::seconds seconds_since_start1 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time_user1.set_seconds(seconds_since_start1.count());
  start_time_user1.set_useconds(useconds_since_start1.count());
  chirp::Timestamp start_time_user2 = start_time_user1;
  start_time_user2.set_useconds(useconds_since_start1.count()+1);
  service_layer.beginstream(kvalidHashtag, ksameUser, start_time_user1);
  service_layer.beginstream(kvalidHashtag, ksameUser, start_time_user2);
  service_layer.chirp(kchirper, kvalidChirpText, "");
  std::deque<chirp::Chirp> chirp_results1 = service_layer.stream(kvalidHashtag, ksameUser, start_time_user1);
  ASSERT_EQ(1, chirp_results1.size());
  ASSERT_EQ(kvalidChirpText, chirp_results1[0].text());
  std::deque<chirp::Chirp> chirp_results2 = service_layer.stream(kvalidHashtag, ksameUser, start_time_user2);
  ASSERT_EQ(1, chirp_results2.size());
  ASSERT_EQ(kvalidChirpText, chirp_results2[0].text());
}

// Tests different cases of hashtag parsing to determine validity
TEST(StreamTest, ValidHashtagParsing) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  const std::string kStreamerFightOn = "Krishna";
  const std::string kStreamerTrojanFamily = "Jill";
  const std::string kChirper = "Barath";
  const std::string khashtagFightOn = "fighton";
  const std::string khashtagTrojanFamily = "trojanfamily";
  const std::string kvalidHashtagEndBySpace = "I #fighton every day.";
  const std::string kvalidHashtagAtStart = "#fighton Trojan buddies!!!";
  const std::string kvalidHashtagEndByNull = "My motto is #fighton";
  const std::string kvalidTwoHashtags = "I like to #fighton with my #trojanfamily";
  const std::string kvalidHashtagMultipleTimes = "#fighton #fighton #fighton";
  chirp::Timestamp start_time;
  std::chrono::seconds seconds_since_start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time.set_seconds(seconds_since_start.count());
  start_time.set_useconds(useconds_since_start.count());
  service_layer.registeruser(kStreamerFightOn);
  service_layer.registeruser(kStreamerTrojanFamily);
  service_layer.registeruser(kChirper);
  service_layer.beginstream(khashtagFightOn, kStreamerFightOn, start_time);
  service_layer.beginstream(khashtagTrojanFamily, kStreamerTrojanFamily, start_time);

  // hashtag correctly terminated by space in middle of string
  service_layer.chirp(kChirper, kvalidHashtagEndBySpace, "");
  std::deque<chirp::Chirp> chirp_results = service_layer.stream(khashtagFightOn, kStreamerFightOn, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidHashtagEndBySpace, chirp_results[0].text());

  // hashtag starting at beginning of string correctly added
  service_layer.chirp(kChirper, kvalidHashtagAtStart, "");
  chirp_results = service_layer.stream(khashtagFightOn, kStreamerFightOn, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidHashtagAtStart, chirp_results[0].text());

  // hashtag terminated by null character correctly added
  service_layer.chirp(kChirper, kvalidHashtagEndByNull, "");
  chirp_results = service_layer.stream(khashtagFightOn, kStreamerFightOn, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidHashtagEndByNull, chirp_results[0].text());

  // multiple different hashtags in chirp are all added to appropriate streamers
  service_layer.chirp(kChirper, kvalidTwoHashtags, "");
  chirp_results = service_layer.stream(khashtagFightOn, kStreamerFightOn, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidTwoHashtags, chirp_results[0].text());
  std::deque<chirp::Chirp> chirp_results2 = service_layer.stream(khashtagTrojanFamily, kStreamerTrojanFamily, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidTwoHashtags, chirp_results[0].text());

  // hashtag multiple times in chirp should only be printed once by streamer
  service_layer.chirp(kChirper, kvalidHashtagMultipleTimes, "");
  chirp_results = service_layer.stream(khashtagFightOn, kStreamerFightOn, start_time);
  ASSERT_EQ(1, chirp_results.size());
  ASSERT_EQ(kvalidHashtagMultipleTimes, chirp_results[0].text());
}

// hashtag added after a user stops streaming is not listed when the user begins streaming once again
TEST(StreamTest, EndStream) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  const std::string kvalidChirper = "Krishna";
  const std::string kvalidStreamer = "Jill";
  const std::string kvalidHashtag = "499isgreat";
  const std::string kvalidChirpText1 = "Hey guys, I just wanted to let you know that #499isgreat";
  const std::string kvalidChirpText2 = "I also wanted to mention once again that #499isgreat";
  service_layer.registeruser(kvalidChirper);
  service_layer.registeruser(kvalidStreamer);
  chirp::Timestamp start_time;
  std::chrono::seconds seconds_since_start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
  std::chrono::microseconds useconds_since_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
  start_time.set_seconds(seconds_since_start.count());
  start_time.set_useconds(useconds_since_start.count());
  service_layer.beginstream(kvalidHashtag, kvalidStreamer, start_time);
  service_layer.chirp(kvalidChirper, kvalidChirpText1, "");
  service_layer.endstream(kvalidHashtag, kvalidStreamer, start_time);
  service_layer.chirp(kvalidChirper, kvalidChirpText2, "");
  service_layer.beginstream(kvalidHashtag, kvalidStreamer, start_time);
  std::deque<chirp::Chirp> chirp_results = service_layer.stream(kvalidHashtag, kvalidStreamer, start_time);
  ASSERT_EQ(0, chirp_results.size());
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
