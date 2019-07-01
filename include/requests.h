#ifndef _REQUESTS_H_
#define _REQUESTS_H_

#include <memory.h>

#include <map>
#include <string>
#include <tuple>

#include <curl/curl.h>

namespace requests {

// Struct to hold data in callback	
struct MemoryStruct {
  char* memory;
  size_t size;
};

/**
 * Make a get API call
 */
char* get(std::string url, 
          struct curl_slist* headers, 
          std::tuple<std::string, std::string> auth = std::make_tuple("BASIC", "admin"));

/**
 * Make a post API call
 */
char* post(std::string url, 
           const char* data, 
           struct curl_slist* headers, 
           std::tuple<std::string, std::string> auth = std::make_tuple("BASIC", "admin"));

/**
 * API call to make a request of any type
 *
 * @param: std::string method
 *       : The API call to make
 * @param: std::string url   
 *       : The url to make request to
 * @param: const char* data
 *       : The data to submit with post request
 * @param: struct curl_slist* headers (struct from curl.h)
 *       : The headers to add to request
 * @param: std::tuple<const char*, const char* auth; default = NULL
 *       : The authentication method/value
 *
 * @return: The data received by request
 */
char* request(std::string method, 
              std::string url, 
              const char* data, 
              struct curl_slist* headers, 
              std::tuple<std::string, std::string> auth = std::make_tuple("BASIC", "admin"));

}; // namespace requests

#endif /* End of file */
