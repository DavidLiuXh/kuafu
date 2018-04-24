#ifndef KUAFU_TIMEUTIL_H_
#define KUAFU_TIMEUTIL_H_

#include <sys/time.h>
#include <ctime>

#include "noncopyable.h"

namespace kuafu {

class TimeUtil : public StaticUtilClass {
public:
   static unsigned long long GetTimeMs();
   static void GetTimeOfDay(struct timeval& now);
};

} // namespace kuafu

#endif // #ifndef KUAFU_TIMEUTIL_H_
