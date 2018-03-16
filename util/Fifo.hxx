#ifndef LUTIL_FIFO_HXX
#define LUTIL_FIFO_HXX

#include <cassert>
#include <boost/shared_ptr.hpp>

#ifdef USING_USTL
# include <ustl.h>
#else
# include <deque>
#endif
#include <lutil/util/AbstractFifo.hxx>

namespace LUtil
{
template < class Msg >
class Fifo : public AbstractFifo
{
   public:
      Fifo();
      virtual ~Fifo();
      
      // Add a message to the fifo.
      void add(boost::shared_ptr<Msg> msg);

      /** Returns the first message available. It will wait if no
       *  messages are available. If a signal interrupts the wait,
       *  it will retry the wait. Signals can therefore not be caught
       *  via getNext. If you need to detect a signal, use block
       *  prior to calling getNext.
       */
      boost::shared_ptr<Msg> getNext();


      /** Returns the next message available. Will wait up to
       *  ms milliseconds if no information is available. If
       *  the specified time passes or a signal interrupts the
       *  wait, this method returns 0. This interface provides
       *  no mechanism to distinguish between timeout and
       *  interrupt.
       */
      boost::shared_ptr<Msg> getNext(int ms);

      /** Returns the next message available and the queue status.
       *  Will not wait if no information is available.
       */
      //boost::shared_ptr<Msg> getNext(bool& hasNext);

      /// delete all elements in the queue
      virtual void clear();

      /// Returns true if a message is available.
      virtual bool messageAvailable() const;

   protected:
      typedef std::deque<boost::shared_ptr<Msg> > FifoQueueType;
      FifoQueueType mFifo;

   private:
      Fifo(const Fifo& rhs);
      Fifo& operator=(const Fifo& rhs);
};


template <class Msg>
Fifo<Msg>::Fifo() : 
   AbstractFifo(0)
{}

template <class Msg>
Fifo<Msg>::~Fifo()
{
   clear();
}

template <class Msg>
void
Fifo<Msg>::clear()
{
   boost::mutex::scoped_lock lock(mMutex);
   FifoQueueType().swap(mFifo);
   assert(mFifo.empty());
   mSize = 0;
}

template <class Msg>
void
Fifo<Msg>::add(boost::shared_ptr<Msg> msg)
{
   boost::mutex::scoped_lock lock(mMutex);
   mFifo.push_back(msg);
   ++mSize;
   mCondition.notify_one();
}

template <class Msg>
boost::shared_ptr<Msg>
Fifo<Msg>::getNext()
{
   boost::mutex::scoped_lock lock(mMutex);

   // Wait util there are messages available.
   while (mFifo.empty())
   {
      mCondition.wait(lock);
   }

   // Return the first message on the fifo.
   //
   boost::shared_ptr<Msg> firstMessage = mFifo.front();
   mFifo.pop_front();
   assert(mSize != 0);
   --mSize;
   return firstMessage;
}

template <class Msg>
boost::shared_ptr<Msg>
Fifo<Msg>::getNext(int ms)
{
#if 0
   const unsigned long long begin(TimeUtil::getTimeMs());
   const unsigned long long end(begin + (unsigned int)(ms)); // !kh! the parameter ms should've been unsigned :(

   Lock lock(mMutex); 

   // Wait until there are messages available
   while (mFifo.empty())
   {
      const long long interval = end - TimeUtil::getTimeMs();
      if(interval <= 0)
      {
         return 0;
      }
      bool signaled = mCondition.wait(mMutex, (unsigned int)interval);
      if (!signaled)
      {
         return 0;
      }
   }

   // Return the first message on the fifo.
   //
   boost::shared_ptr<Msg> firstMessage = mFifo.front();
   mFifo.pop_front();
   assert(mSize != 0);
   --mSize;
#else
  boost::shared_ptr<Msg> firstMessage;
#endif
   return firstMessage;
}

template <class Msg>
bool
Fifo<Msg>::messageAvailable() const
{
   boost::mutex::scoped_lock lock(mMutex); 
   return !mFifo.empty();
}


} // namespace LUtil

#endif // #ifndef LUTIL_FIFO_HXX