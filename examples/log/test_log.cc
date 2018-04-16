#include <thread>

#include "log/kflogger.h"

void TestLog() {
    kuafu::Logger::init(kuafu::Logger::Level::LL_INFO_LOG,
                nullptr,
                "./test_log.log",
                false);

    for (int i = 0; i < 100; ++i) {
        INFO_LOG(__FUNCTION__ << " | " << i);
    }
}

int main(int argc, char* argv[]) {
    std::thread thread1(TestLog);
    std::thread thread2(TestLog);

    thread1.join();
    thread2.join();

    return 0;
}
