#include <gtest/gtest.h>

#include "../cpp/key_value_store_service_impl.h"

// adds a value to the KeyValueStore
TEST(PutGetTest, Simple) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");

  const std::deque<std::string>& values = test_store.get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ(1, values.size());
}
// adds two values to the KeyValueStore under the same key
TEST(PutGetTest, SameKeyTest) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  test_store.put("1testkey", "2testvalue");

  const std::deque<std::string>& values = test_store.get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ("2testvalue", values.at(1));
  ASSERT_EQ(2, values.size());
}
// gets a key without a value
TEST(GetTest, NoValue) {
  KeyValueStore test_store;

  const std::deque<std::string>& values = test_store.get("empty_key");
  ASSERT_EQ(0, values.size());
}

// adds a value to the KeyValueStore and deletes a key with one value
TEST(PutGetDeleteTest, SingleValue) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");

  const std::deque<std::string>& values = test_store.get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ(1, values.size());

  test_store.deletekey("1testkey");
  const std::deque<std::string>& values2 = test_store.get("1testkey");
  ASSERT_EQ(0, values2.size());
}

// adds two values to the KeyValueStore under the same key and deletes the key
TEST(PutGetDeleteTest, MultiValue){
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  test_store.put("1testkey", "2testvalue");

  const std::deque<std::string>& values = test_store.get("1testkey");
  ASSERT_EQ("1testvalue", values.at(0));
  ASSERT_EQ("2testvalue", values.at(1));
  ASSERT_EQ(2, values.size());

  test_store.deletekey("1testkey");
  const std::deque<std::string>& values2 = test_store.get("1testkey");
  ASSERT_EQ(0, values2.size());
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
