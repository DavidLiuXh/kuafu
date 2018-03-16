#include "stdafx.h"

#include <lutil/util/TimeUtil.hxx>

#include <boost/throw_exception.hpp>

#if !_HAS_EXCEPTIONS && defined(_WIN32)
#include <typeinfo>
namespace std 
{
   using ::type_info;
}
#endif

#include <cassert>

using namespace LUtil;

#ifdef _WIN32
void TimeUtil::SystemTimeToCTime(SYSTEMTIME& systemTime, struct tm& cTime)
{
   ::ZeroMemory(&cTime, sizeof(struct tm));
   cTime.tm_hour = systemTime.wHour;
   cTime.tm_min = systemTime.wMinute;
   cTime.tm_sec = systemTime.wSecond;
   cTime.tm_mon = systemTime.wMonth - 1;
   cTime.tm_mday = systemTime.wDay;
   cTime.tm_wday = systemTime.wDayOfWeek;
   cTime.tm_year = systemTime.wYear - 1900;
}
#endif

void 
TimeUtil::getTimeOfDay(struct timeval& now)
{
#ifdef _WIN32
   struct __timeb64 timebuffer;
   _ftime64_s(&timebuffer);
   now.tv_sec = timebuffer.time;
   now.tv_usec = timebuffer.millitm * 1000;
   //boost::xtime xt;
   //boost::xtime_get(&xt, boost::TIME_UTC);
   //now.tv_sec = xt.sec;
   //now.tv_usec = xt.nsec/1000;
#else
   gettimeofday(&now, NULL);
#endif
}

unsigned long long
TimeUtil::getTimeMs()
{
   struct timeval now;
   getTimeOfDay(now);
   unsigned long long time = 0;
   time = now.tv_sec;
   time *= 1000000;
   time += now.tv_usec;
   time /= 1000;

   return time;
}

