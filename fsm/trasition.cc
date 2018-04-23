#include "stdafx.h"

#include <cstring>
#include <cassert>

#include <lutil/fsm/Transition.hxx>

#include <lutil/fsm/Machine.hxx>
#include <lutil/fsm/State.hxx>
#include <lutil/fsm/TransitionPredicate.hxx>


using namespace LUtil;

Transition::Transition(State& from,
                       const char* name,
                       State& to,
                       IPredicate* pred) throw()
                       :mTransitionType(tt_NormalTransition)
                       ,mName(name)
                       ,mPred(pred)
                       ,mTo(0)
                       ,mHoldFrom(&from)
                       ,mHoldTo(&to)
                       ,mIsValid(true)
{
   assert(mPred.get() && "must have predicate");
   init();
}

Transition::Transition(State& from, 
                       State& to, 
                       const Transition& copy) throw()
                       :mTransitionType(copy.mTransitionType)
                       ,mName(copy.mName)
                       ,mPred(copy.mPred)
                       ,mTo(0)
                       ,mHoldFrom(&from)
                       ,mHoldTo(&to)
                       ,mIsValid(false)
{
   copyTransitionCheck(copy);
   init();
}

Transition::Transition(State& toFrom,
                       const char* name,
                       IPredicate* pred) throw()
                       :mTransitionType(tt_InternalTransion)
                       ,mName(name)
                       ,mPred(pred)
                       ,mTo(0)
                       ,mHoldFrom(&toFrom)
                       ,mHoldTo(&toFrom)
                       ,mIsValid(true)
{
   assert(mPred.get() && "must have predicate");
   init();

}

Transition::Transition(State& from,
                       const char* name,
                       State& to,
                       boost::shared_ptr<IPredicate> pred) throw()
                       :mTransitionType(tt_NormalTransition)
                       ,mName(name)
                       ,mPred(pred)
                       ,mTo(0)
                       ,mHoldFrom(&from)
                       ,mHoldTo(&to)
                       ,mIsValid(true)
{
   assert(mPred.get() && "must have predicate");
   init();
}

Transition::Transition(State& toFrom,
                       const char* name,
                       boost::shared_ptr<IPredicate> pred) throw()
                       :mTransitionType(tt_InternalTransion)
                       ,mName(name)
                       ,mPred(pred)
                       ,mTo(0)
                       ,mHoldFrom(&toFrom)
                       ,mHoldTo(&toFrom)
                       ,mIsValid(true)
{
   assert(mPred.get() && "must have predicate");
   init();
}

Transition::Transition(State& toFrom, 
                       const Transition& copy) throw()
                       :mTransitionType(copy.mTransitionType)
                       ,mName(copy.mName)
                       ,mPred(copy.mPred)
                       ,mTo(0)
                       ,mHoldFrom(&toFrom)
                       ,mHoldTo(&toFrom)
                       ,mIsValid(false)
{
   copyTransitionCheck(copy);
   init();
}

Transition::~Transition()
{

}

void
Transition::copyTransitionCheck(const Transition& copy)
{
   if (::strcmp(copy.mHoldFrom->getName(), mHoldFrom->getName()) != 0)
   {
      assert(false);
#ifndef BOOST_NO_EXCEPTIONS
      throw runtime_error("From State not matching to copy transition");
#endif
   }

   if (::strcmp(copy.mHoldTo->getName(), mHoldTo->getName()) != 0)
   {
      assert(false);
#ifndef BOOST_NO_EXCEPTIONS
      throw runtime_error("To State not matching to copy transition");
#endif
   }
   mIsValid = true;
}

void
Transition::init() throw()
{
   if (find(mHoldFrom->mTransitions.begin(),
      mHoldFrom->mTransitions.end(),
      this) == mHoldFrom->mTransitions.end())
   {
      mHoldFrom->mTransitions.push_back(this);
      mTo = mHoldTo;
   }
}

bool
Transition::isMatch(const Event* event,
                    const MachineBase& machine) throw()
{
   if (mPred.get())
   {
      return (*mPred)(event, machine);
   }
   else
   {
      return false;
   }
}

void
Transition::clearActions() throw()
{
   OnTransition.Clear();
}

// NonTransitiveAction
NonTransitiveAction::NonTransitiveAction(ActionMachine& ownerMachine,
                                         const char* name,
                                         IPredicate* pred) throw()
                                         :mName(name)
                                         ,mPred(pred)
{
   assert(mPred.get() && "must have predicate");
   init(ownerMachine);
}

NonTransitiveAction::NonTransitiveAction(ActionMachine& ownerMachine,
                                         const char* name,
                                         boost::shared_ptr<IPredicate> pred) throw()
                                         :mName(name)
                                         ,mPred(pred)
{
   assert(mPred.get() && "must have predicate");
   init(ownerMachine);
}

NonTransitiveAction::NonTransitiveAction(ActionMachine& ownerMachine, 
                                         const NonTransitiveAction& copy) throw()
                                         :mName(copy.mName)
                                         ,mPred(copy.mPred)
{
   init(ownerMachine);
}

NonTransitiveAction::~NonTransitiveAction()
{
}

void NonTransitiveAction::init(ActionMachine& ownerMachine) throw()
{
   ActionMachine::ActionListType::iterator found = 
      find(ownerMachine.mNonTransitiveActions.begin(), ownerMachine.mNonTransitiveActions.end(), this);
   if (found == ownerMachine.mNonTransitiveActions.end())
   {
      ownerMachine.mNonTransitiveActions.push_back(this);
   }
}

//virtual 
bool 
NonTransitiveAction::isMatch(const Event* event,
                             const MachineBase& machine) throw()
{
   if (mPred.get())
      return (*mPred)(event, machine);
   else
      return false;
}

void 
NonTransitiveAction::clearActions() throw()
{
   OnAction.Clear();
}
