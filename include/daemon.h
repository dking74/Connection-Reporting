#ifndef _DAEMON_H
#define _DAEMON_H

#include <unistd.h>

#include <map>

// Define a macro that allow for simplification of calling a pointer-to-member function
#define CALL_MEMBER_FN(object,ptrToMember) ((object)->*(ptrToMember))

/**
 * Responsible for logging to rsyslog on Linux
 *
 * Instantiating an instance of Logger creates a map
 * of keywords to member functions, where each function
 * is responsible for a different type of log message.
 * One could use the higher level member function "log"
 * for higher-level control.
 */
class Logger {
 typedef void (Logger::*log_func)(const char* message);
 std::map<const char*, log_func> log_map;

 public:
  Logger();
  void log_debug(const char* message);
  void log_info(const char* message);
  void log_notice(const char* message);
  void log_warning(const char* message);
  void log_error(const char* message);
  void log_critical(const char* message);
  void log_alert(const char* message);
  void log_emergency(const char* message);

  void log(const char* message_level, const char* message);
};

/**
 * Allows for a daemon process to be created in background
 *
 * A daemon process is one that runs in the background.
 * The constructor sets the name of the log that we wish to write to.
 * The initialize method must be called after constructor in order 
 * to actually start the daemon process.
 */
class Daemon {
 pid_t pid;
 pid_t sid;
 const char* log_name;
 Logger logging;

 public:
  Daemon(const char* log_name) : log_name(log_name) {};
  ~Daemon();
  bool initialize();
  void log(const char*, const char*);

 private:
  bool create_process();
  void close_files();
  void closelog();
};

#endif
