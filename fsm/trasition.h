#ifndef LUTIL_TRANSITION_HXX
#define LUTIL_TRANSITION_HXX

#include <lutil/LUtil.h>
#include <lutil/delegate/delegate.hxx>

#include <boost/shared_ptr.hpp>

namespace LUtil
{

class LUTIL_API Event;
class LUTIL_API State;
class LUTIL_API IPredicate;
class LUTIL_API MachineBase;
class LUTIL_API MachineType;
class LUTIL_API ActionMachine;

class LUTIL_API ITransition
{
public:
   virtual ~ITransition() {}

   virtual bool isMatch(const Event* event,
      const MachineBase& machine) throw() = 0;

   virtual bool isValid() const throw() = 0;

   //virtual boost::shared_ptr<IPredicate> getPredicate() const = 0;
};

typedef LUtil::Delegate<void (
   MachineBase&,           // machine
   State&,       // fromState
   ITransition&,  // transition
   Event*,       // event
   State&)       // toState
> TransitionEvent;

class LUTIL_API Transition : public ITransition
{
   //DECLARE_SCRIPT_CLASS(Transition);

   friend class StateMachine;
public: // declarations
   enum TransitionType
   {
      tt_NormalTransition,
      tt_InternalTransion,
   };

public: // constructors
   /**
   * Construct normal transition
   */
#if defined(_WIN32)
   __declspec(deprecated("Deprecated Warning: should use boost::shared_ptr<IPredicate> instead")) 
#endif
   Transition(State& from,
      const char* name, // must use global data
      State& to,
      IPredicate* pred) throw();

   Transition(State& from,
      const char* name, // must use global data
      State& to,
      boost::shared_ptr<IPredicate> pred) throw();

   Transition(State& from, State& to, const Transition& copy) throw();
   /**
   * Construct internal transition
   */
#if defined(_WIN32)
   __declspec(deprecated("Deprecated Warning: should use boost::shared_ptr<IPredicate> instead")) 
#endif
   Transition(State& toFrom,
      const char* name, // must use global data
      IPredicate* pred) throw();

   Transition(State& toFrom,
      const char* name,
      boost::shared_ptr<IPredicate> pred) throw();

   Transition(State& toFrom, const Transition& copy) throw();
   //Transition(lua_State* L);

   virtual ~Transition();

protected:
   Transition(State& from,
      const char* name, // must use global data
      State& to) throw();

public: // methods
   //// Script Functions
   //SCRIPT_FUNCTION(getName);
   //SCRIPT_FUNCTION(getTransitionType);
   // End Script Functions
   virtual bool isValid() const throw() { return mPred && mIsValid; }
   const char* getName() const throw() { return mName; }
   void clearActions() throw();
   TransitionType getTransitionType() const throw() { return mTransitionType; }

   virtual bool isMatch(const Event* event, const MachineBase& machine) throw();
   //virtual boost::shared_ptr<IPredicate> getPredicate() const { return mPred; }

public: // events
   TransitionEvent OnTransition;

private:
   TransitionType mTransitionType;
   const char* mName;
   boost::shared_ptr<IPredicate> mPred;
   State* mTo;

   State* mHoldFrom;
   State* mHoldTo;

   bool mIsValid;
private:  // no copying
   Transition(const Transition&);
   Transition& operator=(const Transition&);

private: // methods
   void init() throw();
   void copyTransitionCheck(const Transition& copy);
};

typedef LUtil::Delegate<void (MachineBase&,           // machine
                       ITransition&,  // transition
                       Event*)       // event
> ActionEvent;

class LUTIL_API NonTransitiveAction : public ITransition
{
   friend class ActionMachine;
public: // constructors
#if defined(_WIN32)
   __declspec(deprecated("Deprecated Warning: should use boost::shared_ptr<IPredicate> instead")) 
#endif
   NonTransitiveAction(ActionMachine& ownerMachine, 
      const char* name, // must use global data
      IPredicate* pred) throw();
   NonTransitiveAction(ActionMachine& ownerMachine, 
      const char* name, // must use global data
      boost::shared_ptr<IPredicate> pred) throw();
   NonTransitiveAction(ActionMachine& ownerMachine, 
      const NonTransitiveAction& copy) throw();
   ~NonTransitiveAction();

public: // methods
   virtual bool isValid() const throw() { return mPred; }
   const char* getName() const throw() { return mName; }
   void clearActions() throw();
   virtual bool isMatch(const Event* event, const MachineBase& machine) throw();
   //virtual boost::shared_ptr<IPredicate> getPredicate() const { return mPred; }

public: // events
   ActionEvent OnAction;

private: //methods
   void init(ActionMachine& ownerMachine) throw();

private: // members
   const char* mName;
   boost::shared_ptr<IPredicate> mPred;

private:  // no copying
   NonTransitiveAction& operator=(const NonTransitiveAction&);
};

} // namespace LUtil

#endif // #ifndef LUTIL_TRANSITION_HXX
