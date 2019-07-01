#include "timing.h"

#include <iomanip>

namespace timing {

// Locally bring std::chrono into namespace for easier access
using namespace std::chrono;

year_month_day::year_month_day() {
  system_clock::time_point right_now = system_clock::now();

  new(this)year_month_day(right_now);
}

year_month_day::year_month_day(system_clock::time_point point) {
  std::time_t current_time = system_clock::to_time_t(point);
  std::tm* now = std::localtime(&current_time);
    
  new(this)year_month_day(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
}

system_clock::time_point year_month_day::to_time_point() {
  std::tm now = {};
  now.tm_mday = day;
  now.tm_mon = month - 1; 
  now.tm_year = year - 1900;

  std::time_t now_time = mktime(&now);

  return system_clock::from_time_t(now_time);
}

/**
 * Get the next timepoint for the first of the month
 *
 * @param: std::chrono::system_clock::time_point current
 *       : The current time point in time_point representation
 *
 * @rtype: std::chrono::system_clock::time_point
 *       : The time point after a month increase
 */
system_clock::time_point get_next_timepoint(system_clock::time_point current) {
  year_month_day timestamp{current};
  
  timestamp.month++;
  timestamp.day = 1;
  if (timestamp.month > 12) {
    timestamp.month = 1;
    timestamp.year++;
  }

  return timestamp.to_time_point();
}

/**
 * Function to get the next year_month_day from the current point in time
 */
year_month_day get_next_datapoint() {
  system_clock::time_point current_time = system_clock::now();
  system_clock::time_point next_time = get_next_timepoint(current_time);
  year_month_day next(next_time);

  return next;
}

}; // namespace time
