#include "requests.h"

#include <algorithm>
#include <iostream>

#include <boost/algorithm/string/replace.hpp>

namespace requests {

// This is an internal function that is used as a callback function
// to read request data from the buffer and store it in order to 
// allow users to read the data.
static size_t readBuffer(char* contents, int size, int nmemb, void* userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct* mem = (struct MemoryStruct *)userp;
 
  char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    printf("No memory to allocate\n");
    return 0;
  }

  // Store the information from the buffer into the structure
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = '\0';
  return realsize;
}

// This is is an internal method to structure the basics
// of a libcurl request. It takes the curl handle and adds
// callback function to retrieve results and adds http headers
// for all basic requests.
static void base_request (CURL* curl, const std::string url, struct MemoryStruct& chunk, struct curl_slist* headers) {
  // Set the properties of all requests.
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readBuffer);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

// This is an internal method to map a string method for libcurl auth
// to an appropriate long, bit-masked value compatible for curl calls.
static long get_auth_method(std::string method) {
  // Define a mapping for const char* methods to long bitmask repr.
  std::map<std::string, long> auth_methods = {
    {"BASIC",  CURLAUTH_BASIC},
    {"DIGEST", CURLAUTH_DIGEST},
    {"BEARER", CURLAUTH_BEARER},
    {"NTLM",   CURLAUTH_NTLM},
    {"ANY",    CURLAUTH_ANY}
  };

  // Determine if the method entered by user matches one
  // in the defined map. If it is, return that long value.
  // If not, return auth basic for simplicity
  try {
    return auth_methods.at(method);
  } catch (const std::out_of_range& e) {
    return CURLAUTH_BASIC;
  }
}

// This is an internal function that automatically adds the appropriate
// authentication header to a libcurl request.
static void add_auth_header(CURL* curl, std::tuple<std::string, std::string> auth) {
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, get_auth_method(std::get<0>(auth)));
  curl_easy_setopt(curl, CURLOPT_USERPWD, std::get<1>(auth).c_str());
}

char* get(std::string url, 
          struct curl_slist* headers, 
          std::tuple<std::string, std::string> auth) {
  CURL* curl;
  CURLcode res;
	
  // Initialize a curl request and follow through if we are able to
  curl = curl_easy_init();
  if (curl) {
    // Create a structure to hold the memory for the returned data
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    // Remove all spaces from url to make compatible with browser
    boost::replace_all(url, " ", "%20");

    // Define properties of request
    base_request(curl, url, chunk, headers);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    add_auth_header(curl, auth);
        
    // Perform the request and see if we have actual data to return
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? chunk.memory : NULL;
  }

  return NULL;
}

char* post(std::string url, 
           const char* data, 
           struct curl_slist* headers, 
           std::tuple<std::string, std::string> auth) {
  CURL* curl;
  CURLcode res;
	
  // Initialize a curl request and follow through if we are able to
  curl = curl_easy_init();
  if (curl) {
    // Create a structure to hold the memory for the returned data
    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);
    chunk.size = 0;

    // Remove all spaces from url to make compatible with browser
    boost::replace_all(url, " ", "%20");

    // Define properties of request
    base_request(curl, url, chunk, headers);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    add_auth_header(curl, auth);
        
    // Perform the request and see if we have actual data to return
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? chunk.memory : NULL;
  }

  return NULL;
}

char* request(std::string method, 
              std::string url, 
              const char* data, 
              struct curl_slist* headers, 
              std::tuple<std::string, std::string> auth) {
  boost::replace_all(url, " ", "%20");
  std::transform(method.begin(), method.end(), method.begin(), ::toupper);
  
  if (method.compare("GET") == 0) return get(url, headers, auth);
  else if (method.compare("POST") == 0) return post(url, data, headers, auth);
  /*
  else if (method.compare("PUT")) return put(url);
  else if (method.compare("DELETE")) return delete(url);
  */
  else return NULL;
}

}; // namespace requests
