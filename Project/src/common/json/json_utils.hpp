#ifndef JSON_UTILS_HPP
#define JSON_UTILS_HPP

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h> // for stringify JSON

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace Md_Json
{
  const rapidjson::Value* getJsonValue(const rapidjson::Value& inputValue, const std::string& jsonPath);


  std::unique_ptr<rapidjson::Value> parseJson( const std::string& jsonData );
  bool getElement(const rapidjson::Value& value, const std::string& jsonPath, std::string& output);

//MDTMP lame name
  class Wrapper : public std::map<std::string,std::string> {
    public:
      Wrapper();
      bool parse(const rapidjson::Value& inputValue);
      bool parse(const char* json);
  };

  template<class T>
  class WriteWrapper {
      T& writer_;
      const std::map<std::string,std::string>& replaceStr_;
    public:
      WriteWrapper(T& writer, const std::map<std::string,std::string>& replaceStr);
//MDTMP Add these as actual template parameter...
      typedef rapidjson::StringBuffer OutputStream;
      typedef rapidjson::UTF8<>::Ch Ch;

      void Reset(OutputStream& os);
      bool IsComplete() const;
      bool Null();
      bool Bool(bool b);
      bool Int(int i);
      bool Uint(unsigned u);
      bool Int64(int64_t i64);
      bool Uint64(uint64_t u64);
      bool Double(double d);
      bool String(const Ch* str, rapidjson::SizeType length, bool copy = false);
      /* MDTMP
#if RAPIDJSON_HAS_STDSTRING
bool String(const std::basic_string<Ch>& str);
#endif
*/
      bool StartObject();
      bool Key(const Ch* str, rapidjson::SizeType length, bool copy = false);
      bool EndObject(rapidjson::SizeType memberCount = 0);
      bool StartArray();
      bool EndArray(rapidjson::SizeType elementCount = 0);
      bool String(const Ch* str);
      bool Key(const Ch* str);
  };
}

#endif //JSON_UTILS_HPP
