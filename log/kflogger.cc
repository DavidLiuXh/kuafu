#include "log/kflogger.h"

#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

namespace kuafu {

static const char* kKuafuLoggerDefaultFileName = "__kuafu_logger_default__.log";
static const char* kKuafuLoggerDefaultPattern = "[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n";
static const char* kKuafuLoggerDefaultFileAppendername = "__kuafu_logger_file_log__";
static const char* kKuafuLoggerDefaultConsoleAppendername = "__kuafu_logger_console_log__";

static std::atomic<bool> sKuafuLoggerInitFinished(false);
//---------------------------------------------
log4cplus::Logger Logger::sLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("__kuafu_logger__"));

void Logger::init(LUtil::Logger::LOG_LEVEL level,
                  const char* pattern,
                  const char* fileName) {
   bool expected = false;
   if (!sKuafuLoggerInitFinished.compare_exchange_storge(expected, true)) {
       return;
   }

   if (nullptr == fileName ||
      '\0' == fileName[0]) {
      fileName = kKuafuLoggerDefaultFileName;
   }

   if (nullptr == pattern ||
      '\0' == pattern[0]) {
       pattern = kKuafuLoggerDefaultPattern;
   }

   // use std::auto_ptr to log4cplus::Appender::setLayout
   std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
      pattern));
   if (nullptr != layout.get()) {
      log4cplus::SharedAppenderPtr fileAppender(new log4cplus::RollingFileAppender
         (fileName));      
      if (nullptr != fileAppender.get()) {
         fileAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultFileAppendername));
         fileAppender->setLayout(layout);

         sLogger.addAppender(fileAppender);
      }

      log4cplus::SharedAppenderPtr consoleAppender(new log4cplus::ConsoleAppender());      
      if (nullptr != consoleAppender.get()) {
         consoleAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultConsoleAppendername));
         consoleAppender->setLayout(layout);

         sLogger.addAppender(consoleAppender);
      }

      sLogger.setLogLevel(log4cplus::LogLevel(static_cast<int>(level)));
   }
}

} //namespace kuafu
