#include "util/timeutil.h"

namespace kuafu {

void TimeUtil::GetTimeOfDay(struct timeval& now) {
   gettimeofday(&now, NULL);
}

unsigned long long TimeUtil::GetTimeMs() {
   struct timeval now;
   GetTimeOfDay(now);

   unsigned long long time = 0;
   time = now.tv_sec;
   time *= 1000000;
   time += now.tv_usec;
   time /= 1000;

   return time;
}

}//namespace kuafu
