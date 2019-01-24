#include <gtest/gtest.h>
#include "../cpp/key_value_store_service_impl.h"
#include "../cpp/backendStore.grpc.pb.h"
#include "../cpp/backendStore.pb.h"

TEST(PutTest, ReceiveOkStatus){
  chirp::PutRequest request;
  request.set_key("1testkey");
  request.set_key("1testvalue");

  chirp::PutReply reply;
  grpc::ServerContext context;
  KeyValueStoreServiceImpl sample;
  grpc::Status status = sample.put(&context, &request, &reply);

  ASSERT_EQ(status.ok(), true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
