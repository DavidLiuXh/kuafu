#ifndef KUAFU_FIFO_H_
#define KUAFU_FIFO_H_

#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "util/noncopyable.h"

namespace kuafu {

template <class T>
class Fifo : public NonCopyableForAll {
 public:
      Fifo();
      ~Fifo();
      
      void Add(const T& msg);
      void Add(T&& msg);
      T GetNext();
      T GetNext(unsigned int ms);
      void Clear();
      bool MessageAvailable() const;
      bool Empty() const;
      unsigned int Size() const;

   private:
      typedef std::deque<T> FifoQueueType;
      FifoQueueType fifo_;

      mutable std::mutex mutex_;
      std::condition_variable condition_;
};

template <class T>
Fifo<T>::Fifo()
:fifo_() {
}

template <class T>
Fifo<T>::~Fifo() {
   Clear();
}

template <class T>
bool Fifo<T>::Empty() const {
    std::lock_guard<std::mutex> lock(mutex_); 
    return fifo_.empty();
}

template <class T>
unsigned int Fifo<T>::Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<unsigned int>(fifo_.size());
}

template <class T>
void Fifo<T>::Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    FifoQueueType().swap(fifo_);
    condition_.notify_one();
}

template <class T>
void Fifo<T>::Add(const T& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    fifo_.push_back(msg);
    condition_.notify_one();
}

template <class T>
void Fifo<T>::Add(T&& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    fifo_.push_back(std::move(msg));
    condition_.notify_one();
}

template <class T>
T Fifo<T>::GetNext() {
   std::unique_lock<std::mutex> lk(mutex_);
   condition_.wait(lk, [this]() {
               return !fifo_.empty();
               });

   T first_message(std::move(fifo_.front()));
   fifo_.pop_front();

   return first_message;
}

template <class T>
T Fifo<T>::GetNext(unsigned int internal_ms) {
    T first_message;

    auto now = std::chrono::system_clock::now();

    std::unique_lock<std::mutex> lk(mutex_);
    bool signaled = condition_.wait_until(lk,
                now + std::chrono::milliseconds(internal_ms),
                [this]() {
                return !fifo_.empty();
                });
    if (signaled) {
        first_message = std::move(fifo_.front());
        fifo_.pop_front();
    }

    return first_message;
}

template <class T>
bool Fifo<T>::MessageAvailable() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !fifo_.empty();
}

} // namespace kuafu 

#endif // #ifndef KUAFU_FIFO_H_
