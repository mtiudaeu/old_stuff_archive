#include <mat-utils/unit_test/unit_test.hpp>

#include <mat-utils/init/init.hpp>

TEST(InitProgram, init) {

  std::string inputStr;
  bool inputBool = false;
  std::vector<std::string> inputVec;

  mat_utils::init_program initProgram;

  EXPECT_TRUE(initProgram.add("a", mat_utils::init_program::ARGS_ONE, [&inputStr](const std::string& arg){
      inputStr = arg; } ) );
  EXPECT_FALSE(initProgram.add("a", mat_utils::init_program::ARGS_ONE, [&inputStr](const std::string& arg){
      inputStr = arg; } ) );
  EXPECT_TRUE(initProgram.add("b", mat_utils::init_program::ARGS_NONE, [&inputBool](const std::string& ){
      inputBool = true; } ) );
  EXPECT_TRUE(initProgram.add("c", mat_utils::init_program::ARGS_MULTIPLE, [&inputVec](const std::string& arg){
      inputVec.push_back(arg); } ) );

  const char* args_array[] = { "application_name", "-b", "-a", "test1", "-c", "elem1", "elem2", "elem3" };

  EXPECT_TRUE(initProgram.parse(8, args_array));

  EXPECT_EQ(inputStr, "test1");
  EXPECT_EQ(inputBool, true);
  EXPECT_EQ(inputVec[0], "elem1");
  EXPECT_EQ(inputVec[1], "elem2");
  EXPECT_EQ(inputVec[2], "elem3");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

