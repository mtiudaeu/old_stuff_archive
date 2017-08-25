#include <common/client/client.hpp>

#include <common/unit_test/intrusive_unit_test.hpp>
#include <common/unit_test/unit_test.hpp>

#include <iostream>

namespace {

class test_client : public client {
public:
  test_client(const char* host, const char* port) :
    client(host, port) {
  }

  const std::string& received_string() const {
    return data_;
  }

  virtual void ut_handle_read(const std::string& msg) override {
    data_.append(msg);
  }
private:
  std::string data_;
};

} // namespace

TEST(ServerCommunication, ReceivingEcho) {
  try
  {
    const char* host = "127.0.0.1";
    const char* port = "1234";
    test_client c(host, port);

    // FIXME sleep?
    sleep(1);

    const char* msg = "TestClient";
    c.write(msg);
    
    // FIXME sleep?
    // Wait 1 second. 
    sleep(1);

    EXPECT_TRUE(c.received_string() == msg);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    EXPECT_TRUE(false);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

