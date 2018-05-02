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

    typedef std::function<bool (Level)> LogLevelValidateDelegateType;
    typedef std::function<void (std::ostringstream&&)> LogDelegateType;

    LogLevelValidateDelegateType enable_log_delegate;
    LogDelegateType debuglog_delegate;
    LogDelegateType infolog_delegate;
    LogDelegateType warninglog_delegate;
    LogDelegateType errlog_delegate;

    Level level;

#define ExternalInfoLog(msg) \
    do { \
        if (Level::EL_INFO_LOG >= level && infolog_delegate) { \
            std::ostringstream os; \
            {\
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            }\
            infolog_delegate(std::move(os)); \
            os.str(""); \
        } \
    } while(0)

#define ExternalDebugLog(msg) \
    do { \
        if (Level::EL_DEBUG_LOG >= level && debuglog_delegate) { \
            std::ostringstream os; \
            {\
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            }\
            debuglog_delegate(std::move(os)); \
            os.str(""); \
        } \
    } while(0)
#define ExternalWarnLog(msg) \
    do { \
        if (Level::EL_WARNING_LOG >= level && warninglog_delegate) { \
            std::ostringstream os; \
            {\
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            }\
            warninglog_delegate(std::move(os)); \
            os.str(""); \
        } \
    } while(0)
#define ExternalErrorLog(msg) \
    do { \
        if (Level::EL_ERR_LOG >= level && errlog_delegate) { \
            std::ostringstream os; \
            {\
            os << __FILE__ << " | " << __LINE__ << " | " << __FUNCTION__ << " | " << msg; \
            }\
            errlog_delegate(std::move(os)); \
            os.str(""); \
        } \
    } while(0)
};

}// namespace kuafu

#endif //#ifndef KUAFU_EXTERNAL_LOGGER_H_
