#ifndef KUAFU_LOGGER_H_
#define KUAFU_LOGGER_H_

#include <log4cplus/logger.h>

#include "util/NonCopyable.h"
//------------------------------------------------
namespace kuafu {
#define TRACE_LOG(msg) LOG4CPLUS_TRACE(kuafu::Logger::instance(), msg) 
#define INFO_LOG(msg) LOG4CPLUS_INFO(kuafu::Logger::instance(), msg) 
#define DEBUG_LOG(msg) LOG4CPLUS_DEBUG(kuafu::Logger::instance(), msg)
#define WARNING_LOG(msg) LOG4CPLUS_WARN(kuafu::Logger::instance(), msg) 
#define ERR_LOG(msg) LOG4CPLUS_ERROR(kuafu::Logger::instance(), msg)

class Logger {
 public:
     // log level according to log4cplus's loglevel
     enum class Level {
         LL_ALL = 0,
         LL_DEBUG_LOG = 10000,
         LL_INFO_LOG = 20000,
         LL_WARNING_LOG = 30000,
         LL_ERR_LOGOR = 40000,
         LL_OFF_LOG = 60000,
     };

 public:
      static void Logger::init(Level level,
         const char* pattern,
         const char* fileName);

      static void uninit() {
         sLogger.shutdown();
      }

      static log4cplus::Logger& instance() {
         return sLogger;
      }

 private:
      static log4cplus::Logger sLogger;
};

}//namespace kuafu

#endif//#ifndef KUAFU_LOGGER_H_
