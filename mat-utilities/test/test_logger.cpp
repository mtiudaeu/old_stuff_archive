#include <mat-utils/unit_test/unit_test.hpp>

#include <mat-utils/logger/log.hpp>

TEST(LoggerTest, init) {

  EXPECT_EQ(mat_utils::logger::DEBUG, mat_utils::logger::get_level());

  EXPECT_TRUE(mat_utils::logger::is_active(mat_utils::logger::ERROR));
  EXPECT_TRUE(mat_utils::logger::is_active(mat_utils::logger::WARNING));
  EXPECT_TRUE(mat_utils::logger::is_active(mat_utils::logger::INFO));
  EXPECT_TRUE(mat_utils::logger::is_active(mat_utils::logger::DEBUG));

  mat_utils::logger::set_level( mat_utils::logger::ERROR);
  EXPECT_EQ(mat_utils::logger::ERROR, mat_utils::logger::get_level());

  EXPECT_TRUE(mat_utils::logger::is_active(mat_utils::logger::ERROR));
  EXPECT_FALSE(mat_utils::logger::is_active(mat_utils::logger::WARNING));
  EXPECT_FALSE(mat_utils::logger::is_active(mat_utils::logger::INFO));
  EXPECT_FALSE(mat_utils::logger::is_active(mat_utils::logger::DEBUG));

  mat_utils::logger::set_level( mat_utils::logger::OFF);
  EXPECT_EQ(mat_utils::logger::OFF, mat_utils::logger::get_level());
  EXPECT_FALSE(mat_utils::logger::is_active(mat_utils::logger::ERROR));

  mat_utils::logger::set_level( mat_utils::logger::DEBUG);
  LOG_ERROR("log error");
  LOG_WARNING("log warning");
  LOG_INFO("log info");
  LOG_DEBUG("log debug");
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

