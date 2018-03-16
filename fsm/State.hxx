#ifndef LUTIL_STATE_HXX
#define LUTIL_STATE_HXX

#include <lutil/LUtil.h>
#include <lutil/delegate/delegate.hxx>

#include <boost/shared_ptr.hpp>

#ifdef USING_USTL
# include <ustl.h>
#else
# include <vector>
# include <string>
# include <map>
#endif

#include <ctime>

namespace LUtil
{

   class LUTIL_API Transition;
   class LUTIL_API MachineSet;
   class LUTIL_API MachineType;
   class LUTIL_API MachineBase;
   class LUTIL_API StateMachine;
   class LUTIL_API State;
   class LUTIL_API Event;

   typedef LUtil::Delegate<void (
      MachineBase&,     // machine 
      State&)       // state
   > StateEvent;


   class LUTIL_API State
   {
   public: // ctor/dtor
      State(StateMachine& owner, const char* name, time_t timerMS = 0);
      State(StateMachine& owner, const State& copy);
      virtual ~State();

   public: // methods
      const char* getName() const { return mName; }
      void clearActions();
      time_t getTimeout() const;
      void setTimeout(time_t newTimeoutMS);

      bool operator==(const State& rhs) const;
      bool operator!=(const State& rhs) const;

   public: // events
      StateEvent OnEnter;
      StateEvent OnExit;

   private: // methods

   private: // members
      friend class LUTIL_API StateMachine;
      friend class LUTIL_API Transition;

      const char* mName;
      time_t mTimeoutMS;

      vector<Transition*> mTransitions;
   private:
      State& operator=(const State&);
   };

} // namespace LUtil

#endif // #ifndef LUTIL_STATE_HXX
