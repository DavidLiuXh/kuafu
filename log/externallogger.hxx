#ifndef LUTIL_EXTERNAL_LOGGER_HXX
#define LUTIL_EXTERNAL_LOGGER_HXX

#include <lutil/LUtil.h>
#include <lutil/delegate/delegate.hxx>

namespace LUtil
{
  class LUTIL_API ExternalLogger
  {
  public:
    enum Level
    {
      UnKnown = -1,
      Crit = 2,
      Err = 3,
      Warning = 4,
      Info = 6,
      Debug = 7,
    };

    typedef LUtil::Delegate<void (const ostringstream&, const char*, int)> LogDelegate; // msg, __FILE__, __LINE__
    typedef LUtil::Delegate<void (Level, bool&)> LogLevelValidateDelegate; // 

    LogLevelValidateDelegate EnableLogDelegate;
    LogDelegate CritLogDelegate;
    LogDelegate ErrLogDelegate;
    LogDelegate WarningLogDelegate;
    LogDelegate InfoLogDelegate;
    LogDelegate DebugLogDelegate;
#define ECritLog(msg) TRACE_LOG(__FUNCTION__ << " | " << msg)
#define EErrLog(msg) ERR_LOG(__FUNCTION__ << " | " << msg)
#define EWarningLog(msg) WARNING_LOG(__FUNCTION__ << " | " << msg)
#define EInfoLog(msg) INFO_LOG(__FUNCTION__ << " | " << msg)
#define EDebugLog(msg) DEBUG_LOG(__FUNCTION__ << " | " << msg)
  };
}// namespace LUtil

#endif //#ifndef LUTIL_EXTERNAL_LOGGER_HXX