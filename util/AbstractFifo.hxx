#ifndef LUTIL_ABSTRACTFIFO_HXX
#define LUTIL_ABSTRACTFIFO_HXX 

#include <lutil/LUtil.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

namespace LUtil
{
/** First in first out queue template hoist.
 */
class LUTIL_API AbstractFifo// : public FifoStatsInterface
{
   public:
      AbstractFifo(unsigned int maxSize);
      virtual ~AbstractFifo();

      bool empty() const;
            
      /** Get the current size of the fifo. Note you should not use this function
       *  to determine whether a call to getNext() will block or not.
       *  Use messageAvailable() instead.
       */
      unsigned int size() const;

      /// Returns true if a message is available.
      virtual bool messageAvailable() const = 0;

      /// defaults to zero, overridden by TimeLimitFifo<T>
      //virtual time_t timeDepth() const = 0;

      /// remove all elements in the queue (or not)
      virtual void clear() = 0;

      //virtual size_t getCountDepth() const;
      //virtual time_t getTimeDepth() const;

   protected:
      /** Returns the first message available. It will wait if no
       *  messages are available. If a signal interrupts the wait,
       *  it will retry the wait. Signals can therefore not be caught
       *  via getNext. If you need to detect a signal, use block
       *  prior to calling getNext.
       */
      //void* getNext();


      /** Returns the next message available. Will wait up to
       *  ms milliseconds if no information is available. If
       *  the specified time passes or a signal interrupts the
       *  wait, this method returns 0. This interface provides
       *  no mechanism to distinguish between timeout and
       *  interrupt.
       */
      //void* getNext(int ms);

      /** Returns the next message available and the queue status.
       *  Will not wait if no information is available.
       */
      //void* getNext(bool& hasNext);

      unsigned long mSize;
      unsigned long mMaxSize;
      
      mutable boost::mutex mMutex;
      boost::condition_variable mCondition;

   private:
      // no value semantics
      AbstractFifo(const AbstractFifo&);
      AbstractFifo& operator=(const AbstractFifo&);
};

} // namespace LUtil

#endif // #ifndef LUTIL_ABSTRACTFIFO_HXX