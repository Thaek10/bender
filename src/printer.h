#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

template <class T> struct Printer {
  Printer() { to_stdout_ = true; }
  Printer(const std::string filename, bool to_stdout = false) : to_stdout_(to_stdout), filename_(filename) {
    try {
      logger_ = spdlog::basic_logger_st(filename_, "log/" + filename + ".log");
    } catch (const spdlog::spdlog_ex& ex) {
      std::cout << "Log init failed:" << ex.what() << std::endl;
    }
  }

  void Receive(const T& item) const {
    if (logger_) {
      logger_->info(item.String());
    }
    if (to_stdout_) {
      spdlog::info(item.String());
    }
  }

  void operator()(const T& item) const { Receive(item); }

private:
  bool to_stdout_ = false;
  const std::string filename_;
  std::shared_ptr<spdlog::logger> logger_;
};