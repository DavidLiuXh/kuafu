#ifndef LUTIL_TIMEUTIL_HXX
#define LUTIL_TIMEUTIL_HXX

#include <lutil/LUtil.h>

#ifdef _WIN32
#  include <windows.h>
#  include <sys/timeb.h>
#else
#  include <sys/time.h>
#endif

#include <ctime>

namespace LUtil
{

class LUTIL_API TimeUtil
{
public:
#ifdef _WIN32
   static void SystemTimeToCTime(SYSTEMTIME& systemTime, struct tm& cTime);
#endif
   static unsigned long long getTimeMs();
   static void getTimeOfDay(struct timeval& now);

};

} // namespace LUtil

#endif // #ifndef LUTIL_TIMEUTIL_HXX
