#ifndef _TIMING_H
#define _TIMING_H

#include <ctime>
#include <chrono>

namespace timing {

// Locally bring std::chrono into namespace for easier access
using namespace std::chrono;

/**
 * Struct to easily keep track of the year, 
 * month and day in integer form. It also 
 * allows for easy conversion to std::chrono::time_point
 */
struct year_month_day {
  int year;
  int month;
  int day;

  year_month_day();
  year_month_day(int year, int month, int day): 
      year(year), month(month), day(day) { }

  year_month_day(std::chrono::system_clock::time_point point);
  std::chrono::system_clock::time_point to_time_point();
};

/**
 * Get the next timepoint for the first of the month
 *
 * @param: std::chrono::system_clock::time_point current
 *       : The current time point in time_point representation
 *
 * @rtype: std::chrono::system_clock::time_point
 *       : The time point after a month increase
 */
std::chrono::system_clock::time_point get_next_timepoint(std::chrono::system_clock::time_point current);

/**
 * Function to get the next year_month_day from the current point in time
 */
year_month_day get_next_datapoint();

}; // namespace time

#endif