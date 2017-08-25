#include <common/server/server.hpp>
#include <common/server/session_manager.hpp>

#include <common/unit_test/config_unit_test.hpp>
#include <common/unit_test/unit_test.hpp>

TEST(ServerHost, NoExeption) {
  try
  {
    // Initialise the server.
    network::server::server s(unit_test::getServerPort(),
        std::make_shared<network::server::session_manager_echo>());

    // Run the server until stopped.
    s.run();
    EXPECT_TRUE(true);
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
    EXPECT_TRUE(false);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

