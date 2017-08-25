#include <common/json/json_utils.hpp>

#include <common/base/logger/log.hpp>

#include <rapidjson/reader.h>
#include <rapidjson/error/en.h>
//MDTMP #include "rapidjson/stringbuffer.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

const rapidjson::Value* Md_Json::getJsonValue( const rapidjson::Value& inputValue,
    const std::string& jsonPath)
{
  std::vector<std::string> vPath;
  boost::split(vPath, jsonPath, boost::is_any_of("."));

  const rapidjson::Value* currentValue = &inputValue;
  for ( std::vector<std::string>::const_iterator it = vPath.begin(), end = vPath.end();
        it != end ; ++it )
  {
    if ( !currentValue ) { LOG_ERROR("No currentValue."); return 0x0; }

    const char* valueName = it->c_str();
    if ( !currentValue->HasMember(valueName) ) {
      LOG_ERROR("No member of that name : " << valueName);
      return 0x0;
    }

    currentValue = &(*currentValue)[valueName];
  }

  if ( !currentValue ) { LOG_ERROR("No currentValue."); return 0x0; }

  return currentValue;
}

std::unique_ptr<rapidjson::Value> Md_Json::parseJson( const std::string& jsonData )
{
  rapidjson::Document* document(new rapidjson::Document());  // Default template parameter uses UTF8 and MemoryPoolAllocator.
  std::unique_ptr<rapidjson::Value> docPtr(document);

  rapidjson::ParseResult ok = document->Parse(jsonData.c_str());
  if(!ok){
    LOG_ERROR( "Error(offset " << document->GetErrorOffset() << "): "
               << rapidjson::GetParseError_En(ok.Code()) );
    LOG_ERROR("Failed parsing Json string.");
    return 0x0;
  }

  return docPtr;
}

bool Md_Json::getElement(const rapidjson::Value& value,
    const std::string& jsonPath,
    std::string& output)
{
  output.clear();

  const rapidjson::Value* stringValue = getJsonValue(value, jsonPath);
  if(!stringValue) { return false; }
  if(!stringValue->IsString()) { LOG_ERROR("stringValue is not a string."); return false; }

  output = stringValue->GetString();
  return true;
}


Md_Json::Wrapper::Wrapper()
{}

bool Md_Json::Wrapper::parse(const rapidjson::Value& inputValue)
{
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  inputValue.Accept(writer);

  return parse(buffer.GetString());
}

bool Md_Json::Wrapper::parse(const char* json)
{
  struct ReadHandler {
    private:
    Wrapper& wrapper_;

    std::vector<std::string> object_stack_;
    std::string current_key_;

    public:
    ReadHandler(Wrapper& wrapper,
        char object_character = '$') :
      wrapper_(wrapper)
    {}

    bool Null() { return true; }
    bool Bool(bool) { return true; }
    bool Int(int) { return true; }
    bool Uint(unsigned) { return true; }
    bool Int64(int64_t) { return true; }
    bool Uint64(uint64_t) { return true; }
    bool Double(double) { return true; }
    bool String(const char* str, rapidjson::SizeType, bool /*copy*/) { 
      if(!str) { LOG_ERROR("str is null"); return false; }

      Wrapper::iterator lb = wrapper_.lower_bound( str );
      if(lb != wrapper_.end() && lb->first == str) {
        LOG_ERROR("str already exists in map : " << lb->first );
        return false;
      }

      auto concateneStrings = [](const std::vector<std::string>& vec)
      {
        std::string output;
        for(const auto& it:vec) {
          if(!output.empty()) {
            output.append(".");
          }
          output.append(it);
        }
        return output;
      };

      if(current_key_.empty()) {
         LOG_ERROR("current_key_ is empty.")
            return false;
      }
      object_stack_.push_back(current_key_);
      wrapper_.insert(lb, std::make_pair(str, concateneStrings(object_stack_)));
      object_stack_.pop_back();

      return true;
    }
    bool StartObject() {
      if(current_key_.empty()) { return true; }
      object_stack_.push_back(current_key_);
      return true;
    }
    bool Key(const char* str, rapidjson::SizeType, bool /*copy*/) {
      if(!str) { LOG_ERROR("str is null"); return false; }

      current_key_ = str;
      return true;
    }
    bool EndObject(rapidjson::SizeType memberCount) { 
      if(object_stack_.empty()) { return true; }
      object_stack_.pop_back();
      return true;
    }
    bool StartArray() { return true; }
    bool EndArray(rapidjson::SizeType elementCount) { return true; }
  } readHandler(*this);

  if(!json) {
    LOG_ERROR("json is null");
    return false;
  }

  rapidjson::Reader reader;
  rapidjson::StringStream ss(json);
  //MDTMP reader.Parse(ss, readHandler);
  rapidjson::ParseResult ok = reader.Parse(ss, readHandler);
  if(!ok){
    LOG_ERROR( "Error(offset " << reader.GetErrorOffset() 
        << "): " << rapidjson::GetParseError_En(ok.Code()) );
    LOG_ERROR("Failed parsing Json string.");
    return false;
  }

  return true;
}

template<class T>
Md_Json::WriteWrapper<T>::WriteWrapper(T& writer,
                                       const std::map<std::string,std::string>& replaceStr) :
  writer_(writer),
  replaceStr_(replaceStr)
{
}

template<class T>
void Md_Json::WriteWrapper<T>::Reset(OutputStream& os)
{
  writer_.Reset(os);
}
template<class T>
bool Md_Json::WriteWrapper<T>::IsComplete() const
{
  return writer_.IsComplete();
}
template<class T>
bool Md_Json::WriteWrapper<T>::Null()
{
  return writer_.Null();
}
template<class T>
bool Md_Json::WriteWrapper<T>::Bool(bool b)
{
  return writer_.Bool(b);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Int(int i)
{
  return writer_.Int(i);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Uint(unsigned u)
{
  return writer_.Uint(u);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Int64(int64_t i64)
{
  return writer_.Int64(i64);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Uint64(uint64_t u64)
{
  return writer_.Uint64(u64);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Double(double d)
{
  return writer_.Double(d);
}
template<class T>
bool Md_Json::WriteWrapper<T>::String(const Ch* str, rapidjson::SizeType /*length*/, bool copy)
{
  const std::map<std::string,std::string>::const_iterator it = replaceStr_.find(str);
  if ( it == replaceStr_.end() ) { LOG_ERROR("Did not find variable to replace."); return false; }

  const std::string& newValue = it->second;
  return writer_.String(newValue.c_str(), newValue.size(), true/*copy(what should it be here??)*/);
}
template<class T>
bool Md_Json::WriteWrapper<T>::StartObject()
{
  return writer_.StartObject();
}
template<class T>
bool Md_Json::WriteWrapper<T>::Key(const Ch* str, rapidjson::SizeType length, bool copy)
{
  return writer_.Key(str,length,copy);
}
template<class T>
bool Md_Json::WriteWrapper<T>::EndObject(rapidjson::SizeType memberCount)
{
  return writer_.EndObject(memberCount);
}
template<class T>
bool Md_Json::WriteWrapper<T>::StartArray()
{
  return writer_.StartArray();
}
template<class T>
bool Md_Json::WriteWrapper<T>::EndArray(rapidjson::SizeType elementCount)
{
  return writer_.EndArray(elementCount);
}
template<class T>
bool Md_Json::WriteWrapper<T>::String(const Ch* str)
{
  return writer_.String(str);
}
template<class T>
bool Md_Json::WriteWrapper<T>::Key(const Ch* str)
{
  return writer_.Key(str);
}


template class Md_Json::WriteWrapper<rapidjson::Writer<rapidjson::StringBuffer>>;




