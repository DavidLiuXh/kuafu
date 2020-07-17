#include "log/kflogger.h"

#include <atomic>
#include <thread>

#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

#include "util/strutil.h"
#include "util/likely.h"

namespace kuafu {

static constexpr const char* kKuafuLoggerDefaultFileName = "__kuafu_logger_default__.log";
static constexpr const char* kKuafuLoggerDefaultPattern = "[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n";
static constexpr const char* kKuafuLoggerDefaultFileAppendername = "__kuafu_logger_file_log__";
static constexpr const char* kKuafuLoggerDefaultConsoleAppendername = "__kuafu_logger_console_log__";

static std::atomic_flag sKuafuLoggerInitStart(false);
static std::atomic<bool> sKuafuLoggerInitFinished(false);
//---------------------------------------------
log4cplus::Logger* Logger::logger_ = nullptr;

void Logger::Init(Logger::Level level,
                  const char* pattern,
                  const char* file_name,
                  bool output_console) {
    if (sKuafuLoggerInitStart.test_and_set()) {
        while (!sKuafuLoggerInitFinished.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        return;
    }

    static log4cplus::Logger kLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("__kuafu_logger__"));

    if (unlikely(StrUtil::NullOrEmpty(file_name))) {
        file_name = kKuafuLoggerDefaultFileName;
    }

    if (unlikely(StrUtil::NullOrEmpty(pattern))) {
        pattern = kKuafuLoggerDefaultPattern;
    }

    //init RollingFileAppender
    log4cplus::SharedAppenderPtr fileAppender(
        new log4cplus::RollingFileAppender(file_name));      
    if (nullptr != fileAppender.get()) {
        fileAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultFileAppendername));
        // use std::auto_ptr to log4cplus::Appender::setLayout
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        std::auto_ptr<log4cplus::Layout> layout(
            new log4cplus::PatternLayout(pattern));
        if (nullptr != layout.get()) {
            fileAppender->setLayout(layout);
        }
#pragma GCC diagnostic pop

        kLogger.addAppender(fileAppender);
    }

    //init ConsoleAppender 
    if (output_console) {
        log4cplus::SharedAppenderPtr consoleAppender(
            new log4cplus::ConsoleAppender());      
        if (nullptr != consoleAppender.get()) {
            consoleAppender->setName(LOG4CPLUS_TEXT(kKuafuLoggerDefaultConsoleAppendername));
            // use std::auto_ptr to log4cplus::Appender::setLayout
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            std::auto_ptr<log4cplus::Layout> console_layout(
                new log4cplus::PatternLayout(pattern));
            if (nullptr != console_layout.get()) {
                consoleAppender->setLayout(console_layout);
            }
#pragma GCC diagnostic pop

            kLogger.addAppender(consoleAppender);
        }
    }

    kLogger.setLogLevel(log4cplus::LogLevel(static_cast<int>(level)));

    logger_ = &kLogger;

    sKuafuLoggerInitFinished.store(true, std::memory_order_release);
}

} //namespace kuafu
