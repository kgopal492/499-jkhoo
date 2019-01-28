#include <gtest/gtest.h>

#include "../cpp/key_value_store_service_impl.h"
#include "../cpp/backendStore.grpc.pb.h"
#include "../cpp/backendStore.pb.h"

// adds a value to the KeyValueStore
TEST(PutGetTest, Simple) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  const std::deque<std::string>* values = test_store.get("1testkey");
  ASSERT_EQ("1testvalue", values->at(0));
}
// adds two values to the KeyValueStore under the same key
TEST(PutGetTest, SameKeyTest) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  test_store.put("1testkey", "2testvalue");
  //ASSERT_EQ("1testvalue4ba37ad81cc870aa40fb7bd9615a1b8a2testvalue4ba37ad81cc870aa40fb7bd9615a1b8a", test_store.get("1testkey"));
}
// gets a key without a value
TEST(GetTest, NoValue) {
  KeyValueStore test_store;
  //std::string value = test_store.get("empty_key");
  //ASSERT_EQ("", value);
}

// adds a value to the KeyValueStore and deletes a key with one value
TEST(PutGetDeleteTest, SingleValue) {
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  //ASSERT_EQ("1testvalue4ba37ad81cc870aa40fb7bd9615a1b8a", test_store.get("1testkey"));
  test_store.deletekey("1testkey");
  //ASSERT_EQ("", test_store.get("1testkey"));
}

// adds two values to the KeyValueStore under the same key and deletes the key
TEST(PutGetDeleteTest, MultiValue){
  KeyValueStore test_store;
  test_store.put("1testkey", "1testvalue");
  test_store.put("1testkey", "2testvalue");
  //ASSERT_EQ("1testvalue4ba37ad81cc870aa40fb7bd9615a1b8a2testvalue4ba37ad81cc870aa40fb7bd9615a1b8a", test_store.get("1testkey"));
  test_store.deletekey("1testkey");
  //ASSERT_EQ("", test_store.get("1testkey"));
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
