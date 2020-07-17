#ifndef KUAFU_EXTERNAL_LOGGER_H_
#define KUAFU_EXTERNAL_LOGGER_H_

#include <functional>
#include <sstream>

namespace kuafu {

struct ExternalLogger {
    enum class Level {
        EL_ALL = 0,
        EL_DEBUG_LOG,
        EL_INFO_LOG,
        EL_WARNING_LOG,
        EL_ERR_LOG,
        EL_OFF_LOG,
    };

    ExternalLogger()
        :level(Level::EL_INFO_LOG) {
        }

    using LogLevelValidateDelegateType = std::function<bool (Level)>;
    using LogDelegateType = std::function<void (std::ostringstream&)>;

    LogLevelValidateDelegateType enable_log_delegate;
    LogDelegateType debuglog_delegate;
    LogDelegateType infolog_delegate;
    LogDelegateType warninglog_delegate;
    LogDelegateType errlog_delegate;

    Level level;

#define ExtInfoLog(msg) \
    do { \
        if (Level::EL_INFO_LOG >= level && infolog_delegate) { \
            std::ostringstream os; \
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            infolog_delegate(os); \
            os.str(""); \
        } \
    } while(0)

#define ExtDebugLog(msg) \
    do { \
        if (Level::EL_DEBUG_LOG >= level && debuglog_delegate) { \
            std::ostringstream os; \
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            debuglog_delegate(os); \
            os.str(""); \
        } \
    } while(0)
#define ExtWarnLog(msg) \
    do { \
        if (Level::EL_WARNING_LOG >= level && warninglog_delegate) { \
            std::ostringstream os; \
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            warninglog_delegate(os); \
            os.str(""); \
        } \
    } while(0)
#define ExtErrorLog(msg) \
    do { \
        if (Level::EL_ERR_LOG >= level && errlog_delegate) { \
            std::ostringstream os; \
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            errlog_delegate(os); \
            os.str(""); \
        } \
    } while(0)
};

}// namespace kuafu

#endif //#ifndef KUAFU_EXTERNAL_LOGGER_H_
