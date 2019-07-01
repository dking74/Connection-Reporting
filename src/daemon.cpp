#include "daemon.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <stdlib.h>

/**
 * Constructor for logger class.
 * Insert logger log functions into map to 
 * conveniently allow daemon to log
 */
Logger::Logger() {
  this->log_map.insert(std::make_pair("DEBUG", &Logger::log_debug));
  this->log_map.insert(std::make_pair("INFO", &Logger::log_info));
  this->log_map.insert(std::make_pair("NOTICE", &Logger::log_notice));
  this->log_map.insert(std::make_pair("WARNING", &Logger::log_warning));
  this->log_map.insert(std::make_pair("ERROR", &Logger::log_error));
  this->log_map.insert(std::make_pair("CRITICAL", &Logger::log_critical));
  this->log_map.insert(std::make_pair("ALERT", &Logger::log_alert));
  this->log_map.insert(std::make_pair("EMERGENCY", &Logger::log_emergency));
}

/**
 * The below are all logging functions for different log levels
 */
inline void Logger::log_debug(const char* message)     { syslog(LOG_DEBUG, message);   }
inline void Logger::log_info(const char* message)      { syslog(LOG_INFO, message);    }
inline void Logger::log_notice(const char* message)    { syslog(LOG_NOTICE, message);  }
inline void Logger::log_warning(const char* message)   { syslog(LOG_WARNING, message); }
inline void Logger::log_error(const char* message)     { syslog(LOG_ERR, message);     }
inline void Logger::log_critical(const char* message)  { syslog(LOG_CRIT, message);    }
inline void Logger::log_alert(const char* message)     { syslog(LOG_ALERT, message);   }
inline void Logger::log_emergency(const char* message) { syslog(LOG_EMERG, message);   }

/**
 * High-level function for logger to log
 *
 * @param: const char* message_level
 *       : The level for the message to be logged
 * @param: const char* message
 *       : The message to be logged
 *
 * @rtype: void
 */
void Logger::log(const char* message_level, const char* message) {
  if (this->log_map.find(message_level) != this->log_map.end())
    CALL_MEMBER_FN(this, this->log_map[message_level])(message);
  else
    this->log_info(message);
}

Daemon::~Daemon() {
  this->closelog();
}

/** 
 * Fork a new process and place it in the background
 *
 * @rtype: bool
 *       : True if initialization is successful
 */
bool Daemon::initialize() {
  bool new_process = this->create_process();
  if (!new_process)
    return false;  

  umask(0);

  openlog(this->log_name, LOG_PID, LOG_DAEMON);

  this->sid = setsid();
  if (this->sid < 0 || chdir("/") < 0) 
    return false;

  this->close_files();

  return true;
}

/**
 * Log message to common logger for daemon
 *
 * @param: const char* level
 *       : The level of the debug message
 * @param: const char* message
 *       : The message to write to logger
 *
 * @rtype: None
 */
void Daemon::log(const char* level, const char* message) {
  this->logging.log(level, message);
}

/**
 * Create a forked process to segment responsibilities
 *
 * @rtype: None
 */
inline bool Daemon::create_process() {
  this->pid = fork();
  if (pid < 0)
    return false;
  if (pid > 0)
    exit(EXIT_SUCCESS);

  return true;
}

/**
 * Close stdin, stdout, and stderr file descriptors
 * to allow us to write to logs
 *
 * @rtype: None
 */
inline void Daemon::close_files() {
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

/**
 * Close the log file appropriately
 */
inline void Daemon::closelog() {
  ::closelog();
}
