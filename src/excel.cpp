#include "excel.h"

#include "internal.h"

namespace excel {

void AddSheetHeading(lxw_workbook* book, lxw_worksheet* sheet, std::vector<std::string> head) {
  // Create a font for the book and attach it to a format
  lxw_format* heading_format = workbook_add_format(book);
  format_set_bold(heading_format);
  format_set_font_size(heading_format, 16);

  // Write heading elements to sheet
  for (unsigned int i = 0; i < head.size(); i++)
    worksheet_write_string(sheet, 0, i, head[i].c_str(), heading_format);
}

void PlaceDataInSheet(lxw_workbook* book, 
                      rapidjson::Value& data, 
                      const char* sheet_name,
                      std::vector<std::string> head) {
  lxw_worksheet* sheet = workbook_add_worksheet(book, sheet_name);

  // Adjust the column widths to the appropriate width of the data
  worksheet_set_column(sheet, 0, 0, 15, NULL);
  worksheet_set_column(sheet, 1, 4, 25, NULL);

  // Add the heading columns to sheet
  AddSheetHeading(book, sheet, head);

  // Iterate through every value in array, get the lowest-level key-value pairs
  // out, and write the individual values to the current sheet
  int row_num = 1;
  for (rapidjson::SizeType itr = 0; itr < data.Size(); itr++) {
    // Store the low-level key-value pairs into a map
    std::map<std::string, std::string> values = json::PullValues(data[itr]);

    // Iterate through each heading element provided and determine
    // if the data we returned has a key value by that header name.
    // If the header element is available, add the value corresponding
    // to the key to the appropriate headed column in workbook.
    for (auto& header : head) {
      if (values.find(header) != values.end()) {
        std::vector<std::string>::iterator element_pos = 
            std::find(std::begin(head), std::end(head), header);
        worksheet_write_string(
                sheet, row_num, 
                element_pos - head.begin(), 
                values[header.c_str()].c_str(), NULL);
      }
    }

    // Increase the row number so that we print to next row
    row_num++;
  }
}

void HandleMonthlyWebdeskConnections(lxw_workbook* book, int month) {
  // Submit a request to get all connections in the last month
  std::string connection_request = "";
  char* connection_info = convenience::GetData(
                                 connection_request,
                                 std::make_tuple(std::string("NTLM"), 
                                       std::string(""))); // Deleted for security purposes
 
  // Parse the character pointer into json data so that we can manipulate
  rapidjson::Document doc = json::GetJsonData(connection_info);
  rapidjson::Value& connection = doc["value"];

  // Place all connection data retrieved into excel sheet
  PlaceDataInSheet(book, connection, "All Webdesk Connections", {"UserName", "StartDate", "EndDate", "ClientName"});
}

void HandleMonthlyWebdeskSessions(lxw_workbook* book, int month) {
  // Submit a request to get all sessions in the last month
  std::string session_request = "";
  char* session_info = convenience::GetData(
                                 session_request,
                                 std::make_tuple(std::string("NTLM"), 
                                       std::string(""))); // Deleted for security purposes

  // Parse the character pointer into json data so that we can manipulate
  rapidjson::Document doc = json::GetJsonData(session_info);
  rapidjson::Value& session = doc["value"];

  // Place all session data retrieved into excel sheet
  PlaceDataInSheet(book, session, "Unique Webdesk Sessions", {"UserName", "StartDate", "EndDate", "ClientName"});
}

void HandleMonthlyVPNConnections(lxw_workbook* book, int month) {
  std::string connection_request = "";
  char* connection_info = convenience::GetData(
                               connection_request, 
                               std::make_tuple(std::string("BASIC"), 
                                       std::string(""))); // Deleted for security purposes
 
  // Parse the character pointer into json data so that we can manipulate
  rapidjson::Document doc = json::GetJsonData(connection_info);
  rapidjson::Value& connection = doc["results"];

  // Place all session data retrieved into excel sheet
  PlaceDataInSheet(book, connection, "All VPN Sessions", {"UserName", "ConnectedTime", "DisconnectedTime"});
}

bool PlaceDataInBook(lxw_workbook* book) {
  int month_date = convenience::GetCurrentMonth();

  // Get all the data for previous month connections and place
  // each bit of data in their own sheets in the same workbook.
  try {
    HandleMonthlyWebdeskConnections(book, month_date);
    HandleMonthlyWebdeskSessions(book, month_date);
    
    HandleMonthlyVPNConnections(book, month_date);
  } catch(...) { return false; }

  return true;
}

} // namespace excel