#include <common/server/network_message.hpp>

#include <common/unit_test/unit_test.hpp>

//MDTMP 
TEST(NetworkMessage, Encode) {
  const std::string str_to_encode("Hello World!!");
  const std::size_t strSize(str_to_encode.length());

  network_message write_msg;
  // Suceed setting compatible data.
  EXPECT_TRUE(write_msg.set_data(str_to_encode));
  // Fail getting data if not encoded.
  EXPECT_FALSE(write_msg.get_buffer());
 
  write_msg.encode();
  std::string str_encoded("0013");
  str_encoded.append(str_to_encode);
  // Verify encoded result.
  EXPECT_TRUE(str_encoded = write_msg.get_buffer());
  EXPECT_TRUE(write_msg.get_data_size(strSize));
  EXPECT_TRUE(write_msg.get_full_size(strSize + 4));
  
}

TEST(NetworkMessage, Decode) {
  const std::string strToEncode("Hello World!!");
  network_message write_msg;
  EXPECT_TRUE(write_msg.set_data(strToEncode));
  write_msg.encode();
  const char* received_msg = write_msg.get_buffer();

  network_message read_msg;
//MDTMP 
  //EXPECT_TRUE(read_msg.append_encoded_data());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


