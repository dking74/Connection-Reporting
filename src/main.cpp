#include "excel.h"
#include "timing.h"
#include "daemon.h"
#include "mail.h"

#include <vector>
#include <chrono>

using namespace std::chrono;


void generate_report(Daemon daemon, timing::year_month_day& next_point) {
  // Log that a report is being generated and then complete the transaction 
  daemon.log("NOTICE", "Generating Webdesk/VPN report...");
  excel::CreateWorkbook<bool>("", excel::PlaceDataInBook);

  // Send email to user with report attached to it
  std::vector<const char*> to_addresses = {};
  std::vector<const char*> cc_addresses = {};
  std::vector<const char*> attachments = {};
  quickmail mail_obj = mail::initialize_mail(
                                   to_addresses,
                                   cc_addresses,
                                   "", 
                                   "Monthly Webdesk/VPN Reports", 
                                   "Attached are the monthly reports for Webdesk/VPN data.", 
                                   attachments);
  mail::send_mail(mail_obj, "");
}


int main() {
  // Initialize a daemon to run in background
  Daemon daemon{"webdesk_connections"};
  daemon.initialize();

  // Set flag to indicate first time through infinite loop.
  // This is used so that if the daemon is started for the first time
  // and it is still the first of the month, the reports can be generated
  bool first_time = true; 

  // Get the next time we should generate report.
  // Then loop forever to determine if the current time is equal to the
  // next time point so that we can generate VPN/Webdesk reports.
  timing::year_month_day next_point = timing::get_next_datapoint();
  while (true) {
    system_clock::time_point current_time = system_clock::now();
    timing::year_month_day current_point(current_time);

    // Determine if the current time is equal to the next time to report
    // If it is, log that we are reporting, get the next time to report,
    // and then generate the excel report needed.
    if (
        (  current_point.year  == next_point.year 
        && current_point.month == next_point.month
        && current_point.day   == next_point.day) 
        ||
        (  current_point.day   == 1
        && first_time          == true)
       )
      generate_report(daemon, next_point);

    // First time flag needs to be changed after first iteration
    // because the check for the first day of month has already been completed.
    if (first_time) first_time = false;

    sleep(1);
  }

  exit(EXIT_SUCCESS);
}
