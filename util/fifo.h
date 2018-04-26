#ifndef KUAFU_FIFO_H_
#define KUAFU_FIFO_H_

#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "util/noncopyable.h"

namespace kuafu {

template <class Msg>
class Fifo : public NonCopyableForAll {
 public:
     typedef typename std::shared_ptr<Msg> FifoElementType;

 public:
      Fifo();
      ~Fifo();
      
      void Add(FifoElementType msg);

      /** Returns the first message available. It will wait if no
       *  messages are available. If a signal interrupts the wait,
       *  it will retry the wait. Signals can therefore not be caught
       *  via getNext. If you need to detect a signal, use block
       *  prior to calling getNext.
       */
      FifoElementType GetNext();

      /** Returns the next message available. Will wait up to
       *  ms milliseconds if no information is available. If
       *  the specified time passes or a signal interrupts the
       *  wait, this method returns 0. This interface provides
       *  no mechanism to distinguish between timeout and
       *  interrupt.
       */
      FifoElementType GetNext(unsigned int ms);

      /** Returns the next message available and the queue status.
       *  Will not wait if no information is available.
       */

      // delete all elements in the queue
      void Clear();

      // Returns true if a message is available.
      bool MessageAvailable() const;

      bool Empty() const;

      unsigned int Size() const;

   private:
      typedef std::deque<FifoElementType> FifoQueueType;
      FifoQueueType fifo_;

      unsigned long size_;

      mutable std::mutex mutex_;
      std::condition_variable condition_;
};

template <class Msg>
Fifo<Msg>::Fifo() {
}

template <class Msg>
Fifo<Msg>::~Fifo() {
   Clear();
}

template <class Msg>
bool Fifo<Msg>::Empty() const {
    std::lock_guard<std::mutex> lock(mutex_); 
    return size_ == 0;
}

template <class Msg>
unsigned int Fifo<Msg>::Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_;
}

template <class Msg>
void Fifo<Msg>::Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    FifoQueueType().swap(fifo_);
    size_ = 0;
    condition_.notify_one();
}

template <class Msg>
void Fifo<Msg>::Add(FifoElementType msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    fifo_.push_back(msg);
    ++size_;
    condition_.notify_one();
}

template <class Msg>
typename Fifo<Msg>::FifoElementType Fifo<Msg>::GetNext() {
   std::unique_lock<std::mutex> lk(mutex_);

   condition_.wait(lk, [this]() {
               return !fifo_.empty();
               });

   FifoElementType first_message = fifo_.front();
   fifo_.pop_front();
   --size_;

   return first_message;
}

template <class Msg>
typename Fifo<Msg>::FifoElementType Fifo<Msg>::GetNext(unsigned int internal_ms) {
    FifoElementType first_message;

    auto now = std::chrono::system_clock::now();

    std::unique_lock<std::mutex> lk(mutex_);
    bool signaled = condition_.wait_until(lk, now + internal_ms * std::chrono::milliseconds(1), [this]() {
                return !fifo_.empty();
                });
    if (signaled) {
        first_message = fifo_.front();
        fifo_.pop_front();
        --size_;
    }

    return first_message;
}

template <class Msg>
bool Fifo<Msg>::MessageAvailable() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !fifo_.empty();
}

} // namespace kuafu 

#endif // #ifndef KUAFU_FIFO_H_
