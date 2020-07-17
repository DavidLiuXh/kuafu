#ifndef KUAFU_LOGGER_H_
#define KUAFU_LOGGER_H_

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "util/restrict_class.h"
//------------------------------------------------
namespace kuafu {

#define TRACE_LOG(msg) \
  do { \
       log4cplus::Logger* logger = kuafu::Logger::Instance(); \
       if (nullptr != logger) { \
         LOG4CPLUS_TRACE(*logger, msg); \
       } \
  } while(0);

#define INFO_LOG(msg) \
  do { \
       log4cplus::Logger* logger = kuafu::Logger::Instance(); \
       if (nullptr != logger) { \
         LOG4CPLUS_INFO(*logger, msg); \
       } \
  } while(0);

#define DEBUG_LOG(msg) \
  do { \
       log4cplus::Logger* logger = kuafu::Logger::Instance(); \
       if (nullptr != logger) { \
         LOG4CPLUS_DEBUG(*logger, msg); \
       } \
  } while(0);

#define WARNING_LOG(msg) \
   do { \
       log4cplus::Logger* logger = kuafu::Logger::Instance(); \
       if (nullptr != logger) { \
         LOG4CPLUS_WARN(*logger, msg); \
       } \
  } while(0);

#define ERR_LOG(msg) \
  do { \
       log4cplus::Logger* logger = kuafu::Logger::Instance(); \
       if (nullptr != logger) { \
         LOG4CPLUS_ERROR(*logger, msg); \
       } \
  } while(0);

class Logger: public StaticUtilClass {
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
     static void Init(Level level,
                 const char* pattern,
                 const char* file_name,
                 bool output_console);

      static void Uninit() {
        if (nullptr != logger_)  {
          logger_->shutdown();
        }
      }

      //It is forbidden to call this Instance function,
      //it is only used by the macro (i.e INFO_LOG) defined above
      static log4cplus::Logger* Instance() {
         return logger_;
      }

 private:
      static log4cplus::Logger* logger_;
};

}//namespace kuafu

#endif//#ifndef KUAFU_LOGGER_H_
