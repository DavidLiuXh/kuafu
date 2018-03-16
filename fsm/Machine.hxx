#ifndef LUTIL_MACHINE_HXX
#define LUTIL_MACHINE_HXX

#include <lutil/LUtil.h>

#ifdef USING_USTL
# include <ustl.h>
#else
# include <iostream>
# include <vector>
#endif

#include <ctime>

#include <lutil/fsm/MachineType.hxx>
#include <lutil/log/externallogger.hxx>

namespace LUtil
{
class LUTIL_API Event;
class LUTIL_API State;
class LUTIL_API NonTransitiveAction;

class LUTIL_API MachineBase : public ExternalLogger
{
public:
   MachineBase(const MachineType& machineType,
      const string& name)
      :mType(machineType)
      ,mName(name)
   {
   }
   virtual ~MachineBase() {}

public:
   virtual bool process(Event*) = 0;
   virtual void setTimeout(unsigned long long timeoutMS) {}
   virtual unsigned long long getTimeout() const { return 0; }
   virtual bool isTimeout() const { return false; }

public: // methods
   const MachineType& getType() const { return mType; }
   const string& getName() const { return mName; }
   void setName(const string& name) { mName = name; }

protected: // members
   const MachineType mType;
   string mName;
};


class LUTIL_API StateMachine : public MachineBase
{
   friend class State;

public: // ctor/dtor
   StateMachine(const MachineType& type,
      const string& name);
   virtual ~StateMachine(){}

public: // methods
   bool forceState(State& state);
   bool setStartState(State& state);
   bool setMetaState(State& state);
   void clearMetaState();
   //State* getState(const string& name);
   const State* getCurrent() const;

   /**
   * @return NULL is no transition is ever made
   */
   const State* getPrevious() const { return mPrevious; }

   virtual void setTimeout(unsigned long long timeoutMS);
   virtual unsigned long long getTimeout() const;
   virtual bool isTimeout() const;

   virtual bool process(Event*);
   //ostream& output(ostream& str) const;

protected: // ctor
   StateMachine(const StateMachine& rhs);

private: // no copy
   StateMachine& operator=(const StateMachine& rhs);

private: // methods
   bool processNormalStateTransition(Event* event);
   bool processMetaStateTransition(Event* event);

protected: // members
   unsigned long long mTimeoutMS;
   State* mCurrent;
   State* mPrevious;
   
   typedef vector<State*> StateListType;
   StateListType mStates;

   State* mMetaState;
};

class LUTIL_API ActionMachine : public MachineBase
{
   friend class NonTransitiveAction;

public: // ctor/dtor
   ActionMachine(const MachineType& type,
      const string& name)
      :MachineBase(type, name)
   {

   }

public: // methods
   virtual bool process(Event*);

protected: // members
   typedef vector<NonTransitiveAction*> ActionListType;
   ActionListType mNonTransitiveActions;
};

//ostream& operator<<(ostream& str, const MachineBase&);


} // namespace LUtil

#endif // #ifndef LUTIL_MACHINE_HXX
