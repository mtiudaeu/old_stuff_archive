#include <common/json/json_utils.hpp>

#include <common/unit_test/unit_test.hpp>

TEST(Parsing, getElement) {
  const std::string jsonData = " { \"hello\" : \"world\", \"object\" : { \"data1\":\"abcde\"} } ";
  const std::string jsonRequest = "object.data1";

  std::unique_ptr<rapidjson::Value> document(Md_Json::parseJson(jsonData));
  ASSERT_TRUE(document.get());

  std::string output;
  EXPECT_TRUE(Md_Json::getElement(*document, "object.data1", output));
  EXPECT_EQ(output, "abcde");

  EXPECT_TRUE(Md_Json::getElement(*document, "hello", output));
  EXPECT_EQ(output, "world");

  EXPECT_FALSE(Md_Json::getElement(*document, "NotFound", output));
  EXPECT_EQ(output, "");
}

TEST(Parsing, JsonWrapper) {
  const std::string jsonData = " { \"hello\" : \"$1\", \"object\" : { \"data1\":\"$2\", \"object2\" : { \"data2\" : \"$3\"}, \"object3\" : { \"data3\" : \"$4\"} } } ";

  Md_Json::Wrapper json_wrapper;
  json_wrapper.parse(jsonData.c_str());

  Md_Json::Wrapper::iterator it = json_wrapper.find("$1");
  EXPECT_TRUE(it != json_wrapper.end());
  EXPECT_EQ(it->second, "hello");

  it = json_wrapper.find("$2");
  EXPECT_TRUE(it != json_wrapper.end());
  EXPECT_EQ(it->second, "object.data1");

  it = json_wrapper.find("$3");
  EXPECT_TRUE(it != json_wrapper.end());
  EXPECT_EQ(it->second, "object.object2.data2");

  it = json_wrapper.find("$4");
  EXPECT_TRUE(it != json_wrapper.end());
  EXPECT_EQ(it->second, "object.object3.data3");
}

TEST(Parsing, JsonWrapper_2) {
  const std::string jsonData = " { \"hello\" : \"$1\", \"object\" : { \"data1\":\"$2\", \"object2\" : { \"data2\" : \"$3\"} } } ";

  const std::unique_ptr<rapidjson::Value> jsonDoc(Md_Json::parseJson(jsonData.c_str()));
  ASSERT_TRUE(jsonDoc.get());

  const rapidjson::Value* object = Md_Json::getJsonValue(*jsonDoc, "object");
  ASSERT_TRUE(object);

  Md_Json::Wrapper json_wrapper;
  json_wrapper.parse(*object);

  Md_Json::Wrapper::iterator it = json_wrapper.find("$3");
  EXPECT_TRUE(it != json_wrapper.end());
  EXPECT_EQ(it->second, "object2.data2");
}

TEST(Parsing, JsonWriteWrapper) {
  const std::string jsonInput = "{\"hello\":\"$1\",\"object\":{\"data1\":\"$2\",\"object2\":{\"data2\":\"$3\"}}}";
  const std::string jsonOutput = "{\"hello\":\"Var1\",\"object\":{\"data1\":\"Var2\",\"object2\":{\"data2\":\"Var3\"}}}";

  std::map<std::string,std::string> replaceMap;
  replaceMap.insert(std::make_pair("$1","Var1"));
  replaceMap.insert(std::make_pair("$2","Var2"));
  replaceMap.insert(std::make_pair("$3","Var3"));

  const std::unique_ptr<rapidjson::Value> jsonDoc(Md_Json::parseJson(jsonInput.c_str()));
  ASSERT_TRUE(jsonDoc.get());

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  Md_Json::WriteWrapper<rapidjson::Writer<rapidjson::StringBuffer>> write_wrapper(writer, replaceMap);

  jsonDoc->Accept(write_wrapper);

  EXPECT_EQ(jsonOutput, buffer.GetString());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

