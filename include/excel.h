#ifndef _EXCEL_H
#define _EXCEL_H

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

#include "xlsxwriter/xlsxwriter.h"
#include "rapidjson/document.h"

namespace excel {

/**
 * Create an excel workbook the way you want
 *
 * @param: char* workbook_name
 *       : The name to give workbook
 * @param: void (*workbook_action)(lxw_workbook* book))
 *       : Function that performs appropriate action on workbook
 *
 * @rtype: void
 */
template<typename T>
void CreateWorkbook(const char* workbook_name, T (*workbook_action)(lxw_workbook* book)) {
  lxw_workbook* book = workbook_new(workbook_name);
  T action = workbook_action(book);
  workbook_close(book);
}

/**
 * Add a heading to a specific sheet
 *
 * @note: We need book pointer so we can add
 *      : heading font to it
 *
 * @param: lxw_workbook*
 *       : The book so that we can add font to it
 * @param: lxw_worksheet*
 *       : The sheet to add heading to
 * @param: std::vector<std::string>
 *       : The heading elements to add
 *
 * @rtype: void
 */
void AddSheetHeading(lxw_workbook*, lxw_worksheet*, std::vector<std::string>);

/**
 * Create a new sheet and place inputted data to it
 *
 * @param: lxw_workbook*
 *       : The book to place data into it
 * @param: rapidjson::Value& data
 *       : The data to place in book
 * @param: const char* sheet_name
 *       : The name to give the sheet
 *
 * @rtype: void
 */
void PlaceDataInSheet(lxw_workbook*, 
                      rapidjson::Value&, 
                      const char*,
                      std::vector<std::string>);

/**
 * Create a sheet in book for all monthly webdesk connections
 *
 * @param: lxw_workbook*
 *       : The already created book to add sheet to
 * @param: int month
 *       : The integer representation of the month
 *         to get data from
 *
 * @rtype: None
 */
void HandleMonthlyWebdeskConnections(lxw_workbook*, int);

/**
 * Create a sheet in book for all monthly webdesk sessions
 *
 * @param: lxw_workbook*
 *       : The already created book to add sheet to
 * @param: int month
 *       : The integer representation of the month
 *         to get data from
 *
 * @rtype: None
 */
void HandleMonthlyWebdeskSessions(lxw_workbook*, int);

/**
 * Create a sheet in book for all monthly vpn connections
 *
 * @param: lxw_workbook*
 *       : The book to create sheet in
 * @param: int
 *       : The integer representation of the month
 *         to get data from
 *
 * @rtype: None
 */
void HandleMonthlyVPNConnections(lxw_workbook*, int);

/**
 * Get all Webdesk information needed and
 * create excel book of all data
 *
 * @param: lxw_workbook*
 *       : Pointer to libxl book already created
 *
 * @rtype True if successful, False if not
 */
bool PlaceDataInBook(lxw_workbook*);

} // namespace excel

#endif