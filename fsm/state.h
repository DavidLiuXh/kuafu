#ifndef KUAFU_STATE_H_
#define KUAFU_STATE_H_

namespace kuafu {

   class Transition;
   class MachineSet;
   class MachineType;
   class MachineBase;
   class StateMachine;
   class State;
   class Event;

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
