#ifndef LUTIL_THREADPOLICY_HXX
#define LUTIL_THREADPOLICY_HXX

#ifndef _WIN32
#  include <pthread.h>
#  include <cerrno>
#elif defined(HAVE_LIBPTHREAD)
#  include <Windows.h>
#  include <pthread.h>
#else
#  include <Windows.h>
#endif

#include <cassert>

namespace LUtil
{
   class PolicySingleThreaded
   {
   public:
      PolicySingleThreaded()
      {
      }

      ~PolicySingleThreaded()
      {
      }

      void lock()
      {
      }

      void unlock()
      {
      }
   };

   class PolicyMultiThreaded
   {
   public:
      PolicyMultiThreaded()
      {
#if !defined(_WIN32) || defined(HAVE_LIBPTHREAD)
         int rc = pthread_mutex_init(&mId,0);
         (void)rc;
         assert( rc == 0 );
#else
         ::InitializeCriticalSection(&mId);
#endif
      }


      ~PolicyMultiThreaded()
      {
#if !defined(_WIN32) || defined(HAVE_LIBPTHREAD)
         int rc = pthread_mutex_destroy(&mId);
         (void)rc;
         assert( rc != EBUSY );  // currently locked 
         assert( rc == 0 );
#else
         ::DeleteCriticalSection(&mId);
#endif
      }

      void lock()
      {
#if !defined(_WIN32) || defined(HAVE_LIBPTHREAD)
         int  rc = pthread_mutex_lock(&mId);
         (void)rc;
         assert( rc != EINVAL );
         assert( rc != EDEADLK );
         assert( rc == 0 );
#else
         ::EnterCriticalSection(&mId);
#endif
      }

      void unlock()
      {
#if !defined(_WIN32) || defined(HAVE_LIBPTHREAD)
         int rc = pthread_mutex_unlock(&mId);
         (void)rc;
         assert( rc != EINVAL );
         assert( rc != EPERM );
         assert( rc == 0 );
#else
         ::LeaveCriticalSection(&mId);
#endif
      }

   private:
#if !defined(_WIN32) || defined(HAVE_LIBPTHREAD)
      mutable pthread_mutex_t mId;
#else
      CRITICAL_SECTION mId;
#endif
   };

} // namespace LUtil

#endif // #ifndef LUTIL_THREADPOLICY_HXX

