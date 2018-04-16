#include "log/kflogger.h"

#include <atomic>
#include <thread>

#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

namespace kuafu {

static const char* kKuafuLoggerDefaultFileName = "__kuafu_logger_default__.log";
static const char* kKuafuLoggerDefaultPattern = "[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n";
static const char* kKuafuLoggerDefaultFileAppendername = "__kuafu_logger_file_log__";
static const char* kKuafuLoggerDefaultConsoleAppendername = "__kuafu_logger_console_log__";

static std::atomic<bool> sKuafuLoggerInitStart(false);
static std::atomic<bool> sKuafuLoggerInitFinished(false);
//---------------------------------------------
log4cplus::Logger Logger::sLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("__kuafu_logger__"));

void Logger::init(Logger::Level level,
                  const char* pattern,
                  const char* file_name,
                  bool output_console) {
    bool expected = false;
    if (!sKuafuLoggerInitStart.compare_exchange_strong(expected, true)) {
        while (!sKuafuLoggerInitFinished.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        return;
    }

    if (nullptr == file_name ||
                '\0' == file_name[0]) {
        file_name = kKuafuLoggerDefaultFileName;
    }

    if (nullptr == pattern ||
                '\0' == pattern[0]) {
        pattern = kKuafuLoggerDefaultPattern;
    }

    log4cplus::SharedAppenderPtr fileAppender(new log4cplus::RollingFileAppender
                (file_name));      
    if (nullptr != fileAppender.get()) {
        fileAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultFileAppendername));
        // use std::auto_ptr to log4cplus::Appender::setLayout
        std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
                        pattern));
        if (nullptr != layout.get()) {
            fileAppender->setLayout(layout);
        }

        sLogger.addAppender(fileAppender);
    }

    if (output_console) {
        log4cplus::SharedAppenderPtr consoleAppender(new log4cplus::ConsoleAppender());      
        if (nullptr != consoleAppender.get()) {
            consoleAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultConsoleAppendername));
            // use std::auto_ptr to log4cplus::Appender::setLayout
            std::auto_ptr<log4cplus::Layout> console_layout(new log4cplus::PatternLayout(
                            pattern));
            if (nullptr != console_layout.get()) {
                consoleAppender->setLayout(console_layout);
            }

            sLogger.addAppender(consoleAppender);
        }
    }

    sLogger.setLogLevel(log4cplus::LogLevel(static_cast<int>(level)));

    sKuafuLoggerInitFinished.store(true, std::memory_order_release);
}

} //namespace kuafu
