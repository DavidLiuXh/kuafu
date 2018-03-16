#ifndef LUTIL_DELEGATE_HXX
#define LUTIL_DELEGATE_HXX

#ifdef _WIN32
#  include <Windows.h>
#endif

#ifdef USING_USTL
# include <ustl.h>
using namespace ustl;
#else
# include <stdexcept> // for runtime_error
# include <utility> // for pair
using namespace std;
#endif

#include <boost/config.hpp>
#include <lutil/policy/thread_policy.hxx>

namespace LUtil
{
   template<class MT_Policy>
   class LockBlock
   {
   private:
      MT_Policy& _mutex;

   public:
      LockBlock(MT_Policy& mutex)
         :_mutex(mutex)
      {
         _mutex.lock();
      }

      ~LockBlock()
      {
         _mutex.unlock();
      }
   };

template <class TSignature, class MT_Policy = PolicySingleThreaded>
class Delegate; // no body as to be specialized


// Macros for template metaprogramming

#define X_JOIN(a, b) a##b //        X_DO_JOIN(a, b)
//#define X_DO_JOIN(a, b)     X_DO_JOIN2(a, b)
//#define X_DO_JOIN2(a, b)    a##b


#define X_MAKE_PARAMS1_0(t)
#define X_MAKE_PARAMS1_1(t)    t##1
#define X_MAKE_PARAMS1_2(t)    t##1, t##2
#define X_MAKE_PARAMS1_3(t)    t##1, t##2, t##3
#define X_MAKE_PARAMS1_4(t)    t##1, t##2, t##3, t##4
#define X_MAKE_PARAMS1_5(t)    t##1, t##2, t##3, t##4, t##5
#define X_MAKE_PARAMS1_6(t)    t##1, t##2, t##3, t##4, t##5, t##6
#define X_MAKE_PARAMS1_7(t)    t##1, t##2, t##3, t##4, t##5, t##6, t##7
#define X_MAKE_PARAMS1_8(t)    t##1, t##2, t##3, t##4, t##5, t##6, t##7, t##8
#define X_MAKE_PARAMS1_9(t)    t##1, t##2, t##3, t##4, t##5, t##6, t##7, t##8, t##9

#define X_MAKE_PARAMS2_0(t1, t2)
#define X_MAKE_PARAMS2_1(t1, t2)   t1##1 t2##1
#define X_MAKE_PARAMS2_2(t1, t2)   t1##1 t2##1, t1##2 t2##2
#define X_MAKE_PARAMS2_3(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3
#define X_MAKE_PARAMS2_4(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4
#define X_MAKE_PARAMS2_5(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5
#define X_MAKE_PARAMS2_6(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6
#define X_MAKE_PARAMS2_7(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6, t1##7 t2##7
#define X_MAKE_PARAMS2_8(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6, t1##7 t2##7, t1##8 t2##8
#define X_MAKE_PARAMS2_9(t1, t2)   t1##1 t2##1, t1##2 t2##2, t1##3 t2##3, t1##4 t2##4, t1##5 t2##5, t1##6 t2##6, t1##7 t2##7, t1##8 t2##8, t1##9 t2##9

#define X_MAKE_PARAMS1(n, t)         X_JOIN(X_MAKE_PARAMS1_, n) (t)
#define X_MAKE_PARAMS2(n, t1, t2)    X_JOIN(X_MAKE_PARAMS2_, n) (t1, t2)

// Specializations

#define X_DELEGATE_NUM_ARGS	0 // Delegate<R ()>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	1 // Delegate<R (T1)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	2 // Delegate<R (T1, T2)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	3 // Delegate<R (T1, T2, T3)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	4 // Delegate<R (T1, T2, T3, T4)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	5 // Delegate<R (T1, T2, T3, T4, T5)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	6 // Delegate<R (T1, T2, T3, T4, T5, T6)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	7 // Delegate<R (T1, T2, T3, T4, T5, T6, T7)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	8 // Delegate<R (T1, T2, T3, T4, T5, T6, T7, T8)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

#define X_DELEGATE_NUM_ARGS	9 // Delegate<R (T1, T2, T3, T4, T5, T6, T7, T8, T9)>
#include <lutil/delegate/internal/delegateTemplate.hxx>
#undef X_DELEGATE_NUM_ARGS

typedef LUtil::Delegate<void (void)> NotifyEvent;

} // namespace LUtil
#endif // #ifndef _DELEGATE_HXX
