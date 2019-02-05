#ifndef ADD_H
#define ADD_H
#include <gtest/gtest.h>

#include "../cpp/service_layer_service_impl.h"
#include "../cpp/service.h"
#include <google/protobuf/util/time_util.h>

// Registers a user and confirms that user key values are correct
TEST(RegisterUserTest, Simple) {
  KeyValueStore test_store;
  ASSERT_EQ(-1, -1);
  ServiceLayer service_layer(&test_store);
  ASSERT_EQ(0, 0);
  service_layer.registeruser("testUser");
  ASSERT_EQ(1, 1);
  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());
}

// registers two users and makes one follow the other
TEST(RegisterUserTest, RegisterRegisterFollow) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  service_layer.registeruser("testUser");

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());

  const std::deque<std::string>& user_following = test_store.get("1testUser");
  ASSERT_EQ(1, user_following.size());

  service_layer.registeruser("testUser2");

  const std::deque<std::string>& user_value_2 = test_store.get("0testUser2");
  ASSERT_EQ(1, user_value_2.size());

  const std::deque<std::string>& user_following_2 = test_store.get("1testUser2");
  ASSERT_EQ(1, user_following_2.size());

  service_layer.follow("testUser", "testUser2");
  const std::deque<std::string>& user_following_now = test_store.get("1testUser");
  ASSERT_EQ(2, user_following_now.size());
  ASSERT_EQ("testUser2", user_following_now.at(1));
}

// register a user, chirp from that user, and then read that chirp
TEST(RegisterUserTest, RegisterChirpRead) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  service_layer.registeruser("testUser");

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
  service_layer.registeruser("testUser");

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

// register two users, have the first follow the second, get a current timestamp, second user chirps twice, monitor starting from timestamp
TEST(FollowingUserTest, FollowMonitor) {
  KeyValueStore test_store;
  ServiceLayer service_layer(&test_store);
  service_layer.registeruser("testUser");
  service_layer.registeruser("testUserToFollow");

  const std::deque<std::string>& user_value = test_store.get("0testUser");
  ASSERT_EQ(1, user_value.size());
  const std::deque<std::string>& user_value2 = test_store.get("0testUserToFollow");
  ASSERT_EQ(1, user_value2.size());

  service_layer.follow("testUser", "testUserToFollow");
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


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#endif
