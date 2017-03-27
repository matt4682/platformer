#ifndef LOG_H__
#define LOG_H__

#include <functional>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iostream>

using StreamFn = std::ostream &(*)(std::ostream &);

enum LogType {
  UNKNOWN,
  INFO,
  DEBUG,
  ERROR
};

////////////////////////////////////////////////////////////////////////////////

class LogFile {
public:
  std::ofstream m_log_file{ "log.txt" };
  LogFile &operator<<(StreamFn fn);
};

template <typename T>
LogFile &operator<<(LogFile &os, const T &data) {
  std::cout << data;
  os.m_log_file << data;

  return os;
}

////////////////////////////////////////////////////////////////////////////////

class Logger {
public:
  Logger();

  LogFile &operator() (LogType type = UNKNOWN);
  
private:
  Logger(const Logger &other) = delete;
  Logger(Logger &&other) = delete;
  Logger &operator=(const Logger &other) = delete;
  Logger &operator=(Logger &&other) = delete;

  LogFile m_logger;
};

extern Logger gamelog;

#endif  // LOG_H__