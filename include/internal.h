#ifndef _INTERNAL_H
#define _INTERNAL_H

#include <map>
#include <string>
#include <tuple>

#include "rapidjson/document.h"

namespace json {

/**
 * Convert string data into json data that can directly called
 *
 * @param: char* json
 *       : The string data to parse 
 *
 * @rtype: rapidjson::Document
 *       : The parsed document 
 */
rapidjson::Document GetJsonData(char* json);
void PrintJsonArray(rapidjson::Value& value);

/**
 * Convenience function to get a list of values from a base dictionary
 *
 * @param: json::Value& dict_obj
 *       : The base dictionary to get info from
 * @param: std::list<std::string> values
 *       : The list of values to return to user
 *
 * @rtype: Map of lower-level values
 */
std::map<std::string, std::string> 
PullValues(rapidjson::Value& dict_obj, std::map<std::string, std::string> values={});

} // namespace json 

namespace convenience {

/**
 * Get the current month at this very moment
 *
 * @rtype: int
 *       : Int repr. of the month
 */
int GetCurrentMonth();

/**
 * Get json data via API request from Webdesk
 *
 * @param: std::string url
 *       : The url to submit to
 *
 * @rtype: char*
 *       : The json data as a character array
 */
char* GetData(std::string url, 
          std::tuple<std::string, std::string> auth = std::make_tuple("BASIC", "admin"));

} // namespace convenience

#endif
