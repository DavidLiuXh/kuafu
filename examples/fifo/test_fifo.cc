#include <thread>
#include <chrono>
#include <memory>
#include <iostream>

#include "util/fifo.h"

void Consume(kuafu::Fifo<int>& fifo) {
    kuafu::Fifo<int>::FifoElementType element;
    while (element = fifo.GetNext()) {
        std::cout << *element << std::endl;

        if (*element == 1000) {
            break;
        }
    }
}

void Consume2(kuafu::Fifo<int>& fifo) {
    kuafu::Fifo<int>::FifoElementType element;
    while (true) {
        element = fifo.GetNext(200);
        if (element) {
            std::cout << *element << std::endl;

            if (*element == 1000) {
                break;
            }
        } else {
            std::cout << "Timeout..." << std::endl;
        }
    }
}

void Produce(kuafu::Fifo<int>& fifo) {
    for (int i = 0; i < 100; ++i) {
        fifo.Add(std::make_shared<int>(i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    fifo.Add(std::make_shared<int>(1000));
}

int main(int argc, char* argv[]) {
    kuafu::Fifo<int> fifo;

    std::thread thread_consumer(Consume2, std::ref(fifo));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread thread_producer(Produce, std::ref(fifo));

    thread_producer.join();
    thread_consumer.join();

    return 0;
}
