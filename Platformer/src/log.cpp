#include "./log.h"

Logger gamelog;

////////////////////////////////////////////////////////////////////////////////

LogFile &LogFile::operator<<(StreamFn fn) {
  fn(m_log_file);
  fn(std::cout);
  return *this;
}

////////////////////////////////////////////////////////////////////////////////

Logger::Logger() {
}

LogFile &Logger::operator() (LogType type) {
  auto time = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(time);
  std::string time_stamp = std::ctime(&current_time);
  time_stamp.erase(time_stamp.find('\n', 0), 1);

  m_logger << "[" << time_stamp << "] ";
  switch (type) {
    case UNKNOWN:
      m_logger << "(UNKNOWN)";
      break;
    case INFO:
      m_logger << "(INFO)";
      break;
    case DEBUG:
      m_logger << "(DEBUG)";
      break;
    case ERROR:
      m_logger << "(ERROR)";
      break;
    default: break;
  }

  return m_logger << ": ";
}