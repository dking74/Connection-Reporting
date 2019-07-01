#include "internal.h"

#include <ctime>
#include <iostream>

#include <curl/curl.h>
#include "requests.h"

namespace json {

rapidjson::Document GetJsonData(char* json) {
  rapidjson::Document document;
  document.Parse(json);

  return document;
}

void PrintJsonArray(rapidjson::Value& value) {
  for (auto& c : value.GetArray()) {
    for (auto& m : c.GetObject())
      std::cout << "Name: " << m.name.GetString() << "; Type: " << m.value.GetType() << std::endl;
    std::cout << std::endl;
  }
}

std::map<std::string, std::string> 
PullValues(rapidjson::Value& dict_obj, std::map<std::string, std::string> values) {
  for (auto& element : dict_obj.GetObject()) {
    std::string element_name = element.name.GetString();
    rapidjson::Value& current_val = dict_obj[element_name.c_str()];

    if (current_val.IsObject()) {
      values = PullValues(current_val, values);
    } else if (current_val.IsArray()) {
      for (rapidjson::SizeType i = 0; i < current_val.Size(); i++)
        values = PullValues(current_val[i], values);
    } else if (current_val.IsString()) {
      values[element_name] = current_val.GetString();
    }
  }

  return values;
}

} // namespace json 

namespace convenience {

int GetCurrentMonth() {
  std::time_t t = std::time(NULL);
  std::tm* now = std::localtime(&t);
  int month = now->tm_mon + 1;

  return month;
}

char* GetData(std::string url, std::tuple<std::string, std::string> auth) {
  // Create a curl list for headers to add to request
  struct curl_slist* base_header = NULL;
  base_header = curl_slist_append(base_header, "Content-Type: application/json");
  base_header = curl_slist_append(base_header, "Accept: application/json");

  char* json = requests::get(url, base_header, auth);
  
  return json;
}

} // namespace convenience
