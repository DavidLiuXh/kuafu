#include <thread>
#include <iostream>
#include <sstream>

#include "log/kflogger.h"
#include "log/externallogger.h"

void TestLog() {
    kuafu::Logger::init(kuafu::Logger::Level::LL_DEBUG_LOG,
                nullptr,
                "./test_log.log",
                false);

    for (int i = 0; i < 100; ++i) {
        INFO_LOG(__FUNCTION__ << " | " << i);
    }
}

void TestLogForMultiThread() {
    std::thread thread_1(TestLog);
    std::thread thread_2(TestLog);

    thread_1.join();
    thread_2.join();
}

class MyLogger: public kuafu::ExternalLogger {
    public:
        void ShowMsg(const char* msg) {
            ExternalInfoLog("hello world!" << " | " << " ---- ");
            ExternalDebugLog("hello world!");
            ExternalWarnLog("hello world!");
            ExternalErrorLog("hello world!" << " | " << " ---- ");
        }
};

void TestLogForExternalLogger() {
    kuafu::Logger::init(kuafu::Logger::Level::LL_DEBUG_LOG,
                nullptr,
                "./test_log.log",
                false);

    MyLogger my_logger;
    my_logger.level = kuafu::ExternalLogger::Level::EL_WARNING_LOG;
   
    my_logger.infolog_delegate = [&](std::ostringstream&& os) {
        INFO_LOG(os.str());
    };
    my_logger.debuglog_delegate = [&](std::ostringstream&& os) {
        DEBUG_LOG(os.str());
    };
    my_logger.errlog_delegate = [&](std::ostringstream&& os) {
        ERR_LOG(os.str());
    };
    my_logger.warninglog_delegate = [&](std::ostringstream&& os) {
        WARNING_LOG(os.str());
    };

    my_logger.ShowMsg("");
}

int main(int argc, char* argv[]) {
    std::cout << "test log for multithread" << std::endl;
    TestLogForMultiThread();
    std::cout << "------------------------" << std::endl;

    std::cout << "test external logger" << std::endl;
    TestLogForExternalLogger();
    std::cout << "------------------------" << std::endl;

    return 0;
}
