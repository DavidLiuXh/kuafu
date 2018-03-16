#ifndef LUTIL_LOGGER_HXX
#define LUTIL_LOGGER_HXX

#include <lutil/LUtil.h>

#include <boost/noncopyable.hpp>

#include <log4cplus/logger.h>
//------------------------------------------------
namespace LUtil
{
#define TRACE_LOG(msg) LOG4CPLUS_TRACE(LUtil::Logger::instance(), msg) 
#define INFO_LOG(msg) LOG4CPLUS_INFO(LUtil::Logger::instance(), msg) 
#define DEBUG_LOG(msg) LOG4CPLUS_DEBUG(LUtil::Logger::instance(), msg)
#define WARNING_LOG(msg) LOG4CPLUS_WARN(LUtil::Logger::instance(), msg) 
#define ERR_LOG(msg) LOG4CPLUS_ERROR(LUtil::Logger::instance(), msg)

   class LUTIL_API Logger
   {
   public:
      enum LOG_LEVEL
      {
         LL_ALL = 0,
         LL_DEBUG_LOG = 10000,
         LL_INFO_LOG = 20000,
         LL_WARNING_LOG = 30000,
         LL_ERR_LOGOR = 40000,
         LL_OFF_LOG = 60000,
      };
   public:
#ifdef _WIN32
      static void Logger::init(LOG_LEVEL level,
         const wchar_t* pattern,
         const wchar_t* fileName);
#else
      static void Logger::init(LOG_LEVEL level,
         const char* pattern,
         const char* fileName);
#endif
      static void uninit()
      {
         sLogger.shutdown();
      }
      static log4cplus::Logger& instance()
      {
         return sLogger;
      }

   private:
      static log4cplus::Logger sLogger;
   };
}//namespace LUtil

#endif//#ifndef LUTIL_LOGGER_HXX