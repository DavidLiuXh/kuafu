#include "LLogger.hxx"

#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

using namespace LUtil;

#ifdef _WIN32
const wchar_t* DEFAULT_LOG_FILE_NAME = L"__lutil_logger__.log";
#else
const char* DEFAULT_LOG_FILE_NAME = "__lutil_logger__.log";
#endif
//---------------------------------------------
log4cplus::Logger LUtil::Logger::sLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("__lutil_logger__"));

#ifdef _WIN32
void Logger::init(LUtil::Logger::LOG_LEVEL level,
                  const wchar_t* pattern,
                  const wchar_t* fileName)
#else
void Logger::init(LUtil::Logger::LOG_LEVEL level,
                  const char* pattern,
                  const char* fileName)
#endif
{
   if (NULL == fileName ||
      '\0' == fileName[0])
   {
      fileName = DEFAULT_LOG_FILE_NAME;
   }

   if (NULL == pattern ||
      '\0' == pattern[0])
   {
      pattern = L"[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n";
   }

   std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
      pattern));
   if (NULL != layout.get())
   {
      log4cplus::SharedAppenderPtr fileAppender(new log4cplus::RollingFileAppender
         (fileName));      
      if (NULL != fileAppender.get())
      {
         fileAppender->setName(LOG4CPLUS_TEXT("file log"));
         fileAppender->setLayout(layout);

         sLogger.addAppender(fileAppender);
      }

      log4cplus::SharedAppenderPtr consoleAppender(new log4cplus::ConsoleAppender());      
      if (NULL != consoleAppender.get())
      {
         consoleAppender->setName(LOG4CPLUS_TEXT("console log"));
         std::auto_ptr<log4cplus::Layout> layout(new log4cplus::PatternLayout(
            //"[%p] [%D{%m/%d/%y %H:%M:%S:%s}] [%t] [%l] - %m %n"));
            LOG4CPLUS_TEXT("[%p] [%D{%m/%d/%y %H:%M:%S,%q}] [%t] [%l] - %m %n")));
         if (NULL != layout.get())
         {
            consoleAppender->setLayout(layout);
         }

         sLogger.addAppender(consoleAppender);
      }

      sLogger.setLogLevel(log4cplus::LogLevel(level));
   }
}