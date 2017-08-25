/*
 * Copyright (c) 2009-2014 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */
#include <common/json/json_utils.hpp>

#include <common/base/init/init.hpp>
#include <common/base/logger/log.hpp>

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include <curl/curl.h>

#include <unistd.h>

#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */

enum RequestType {
   GET_REQUEST,
   PUT_REQUEST,
   POST_REQUEST 
};

struct write_result
{
    char *data;
    int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

static const char *httpRequest(const std::string& url,
                               const std::string& header,
                               RequestType type,
                               const char* dataToSend = 0x0)
{
    CURL *curl = NULL;
    CURLcode status;
    struct curl_slist *headers = NULL;
    char *data = NULL;
    long code;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl)
        goto error;

    data = (char*)malloc(BUFFER_SIZE);
    if(!data)
        goto error;

    write_result write_result;
    write_result.data = data;
    write_result.pos = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* GitHub commits API v3 requires a User-Agent header */
    headers = curl_slist_append(headers, "User-Agent: Md_getJsonPath");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

    switch(type) {
       case PUT_REQUEST:
          if(!dataToSend) {
             LOG_ERROR("Invalid dataToSend.");
             goto error;
          }

          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataToSend);
 
          break;
       case POST_REQUEST:
          if(!dataToSend) {
             LOG_ERROR("Invalid dataToSend.");
             goto error;
          }

          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
          curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataToSend);
 
          break;
          break;
       case GET_REQUEST:
       default:
          break;
    };

    status = curl_easy_perform(curl);
    if(status != 0)
    {
        fprintf(stderr, "error: unable to request data from %s:\n", url.c_str());
        fprintf(stderr, "%s\n", curl_easy_strerror(status));
        goto error;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
//MDTMP Do better error handling. Should depend on GET,PUT or POST
    if(code != 200 && code != 204)
    {
        fprintf(stderr, "error: server responded with code %ld\n", code);
        goto error;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    /* zero-terminate the result */
    data[write_result.pos] = '\0';

    return data;

error:
    if(data)
        free(data);
    if(curl)
        curl_easy_cleanup(curl);
    if(headers)
        curl_slist_free_all(headers);
    curl_global_cleanup();
    return NULL;
}
//MDTMP review code above this line.

static int getRequest(const std::string& url,
                      const std::string& header,
                      const rapidjson::Value& inputJson,
                      bool displayWebResponce)
{
  const char *text = httpRequest(url, header, GET_REQUEST);
  if(!text) {
    LOG_ERROR("httpRequest failed.");
    return 1;
  }

  const std::unique_ptr<rapidjson::Value> responceJson(Md_Json::parseJson(text));
  if(!responceJson) {
    LOG_ERROR("Parsing failed.");
    return 1;
  }

  if(displayWebResponce) {
     //MDTMP implement write with proper indentation.
     rapidjson::StringBuffer buffer;
     rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
     responceJson->Accept(writer);

     printf(buffer.GetString());
     return 0;
  }

  const rapidjson::Value* jsonRequest = Md_Json::getJsonValue(inputJson, "jsonRequest");
  if ( !jsonRequest ) { LOG_ERROR("No jsonRequest in input file."); return 1; }
  if ( !jsonRequest->IsObject() ) { LOG_ERROR("jsonRequest in is not an object."); return 1; }

  Md_Json::Wrapper json_wrapper;
  json_wrapper.parse(*jsonRequest);
  for(const auto& it:json_wrapper) {
    const rapidjson::Value* value = Md_Json::getJsonValue(*responceJson, it.second);
    if ( !value ) { LOG_ERROR("No value \"" << it.second << "\" in responceJson."); return 1; }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value->Accept(writer);

    std::cout << it.first << " : " << buffer.GetString() << std::endl;
  }

  return 0;
}

static int sendRequest(const std::string& url,
                       const std::string& header,
                       const rapidjson::Value& inputJson,
                       const std::vector<std::string> arguments,
                       const RequestType type)
{
  const rapidjson::Value* sendJsonData = Md_Json::getJsonValue(inputJson, "sendJsonData");
  if(!sendJsonData) { LOG_ERROR("No sendJsonData in input file."); return 1; }
  if(!sendJsonData->IsObject()) { LOG_ERROR("sendJsonData in is not an object."); return 1; }

  Md_Json::Wrapper json_wrapper;
  json_wrapper.parse(*sendJsonData);

  if(json_wrapper.size() != arguments.size()) {
    LOG_ERROR("json_wrapper (" << json_wrapper.size() << ") and arguments "
              << arguments.size() << ") size differ.");
    return 1;
  }

  Md_Json::Wrapper::iterator itMap = json_wrapper.begin();
  std::vector<std::string>::const_iterator itVec = arguments.begin();
  const std::vector<std::string>::const_iterator endVec = arguments.end();
  for(; itVec != endVec; ++itMap, ++itVec) {
    itMap->second = *itVec;
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  Md_Json::WriteWrapper<rapidjson::Writer<rapidjson::StringBuffer>> write_wrapper(writer, json_wrapper);

  sendJsonData->Accept(write_wrapper);

  const char *text = httpRequest(url, header, type, buffer.GetString());
  if(!text) {
    LOG_ERROR("httpRequest failed.");
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  std::string inputFileName;
  std::string appendToUrl;
  bool displayWebResponce = false;
  std::vector<std::string> requestArguments;

  Md_DefaultInitProgram initProgram;
  bool status = true;

  status &= initProgram.add("f", Md_InitProgram::ARGS_ONE, [&inputFileName](const std::string& arg){
      inputFileName = arg; } );
  status &= initProgram.add("a", Md_InitProgram::ARGS_ONE, [&appendToUrl](const std::string& arg){
      appendToUrl = arg; } );
  status &= initProgram.add("d", Md_InitProgram::ARGS_NONE, [&displayWebResponce](const std::string& /*arg*/){
      displayWebResponce = true; } );
  status &= initProgram.add("p", Md_InitProgram::ARGS_ONE, [&requestArguments](const std::string& arg){
      requestArguments.push_back(arg); } );
  if(!status) { LOG_ERROR("Failure to initialize program arguments."); return 1; }

  status = initProgram.init(argc, argv);
  if(!status) { LOG_ERROR("Failure to parse program arguments."); return 1; }


  if(inputFileName.empty()) {
     LOG_ERROR( "No inputFileName." );
     return 1;
  }

  std::string line;
  std::ifstream inputFile(inputFileName);
  if(!inputFile.is_open()) {
     LOG_ERROR( "Failed opening : " << inputFileName );
     return 1;
  }

  std::string inputJsonStr;
  while(std::getline(inputFile, line)) {
    inputJsonStr.append(line);
  }
  inputFile.close();

  const std::unique_ptr<rapidjson::Value> inputJson(Md_Json::parseJson(inputJsonStr));
  if(!inputJson) {
    LOG_ERROR("Parsing failed.");
  }

  std::string url;
  if(!Md_Json::getElement(*inputJson, "url", url)){
    LOG_ERROR("Missing url in input file.");
    return 1;
  }
  if(!appendToUrl.empty()) {
     url.append(appendToUrl);
  }

  std::string header;
  Md_Json::getElement(*inputJson, "header", header);

  std::string requestType;
  if(!Md_Json::getElement(*inputJson, "requestType", requestType)){
    LOG_ERROR("Missing requestType in input file.");
    return 1;
  }

  if(requestType == "GET") {
     return getRequest(url, header, *inputJson, displayWebResponce);
  }
  else if(requestType == "PUT") {
     return sendRequest(url, header, *inputJson, requestArguments, PUT_REQUEST);
  }
  else if(requestType == "POST") {
     return sendRequest(url, header, *inputJson, requestArguments, POST_REQUEST);
  }
  LOG_ERROR("Invalid request type.");
  return 1;

}


