#include "fsm/event.h"
#include "fsm/macine.h"
#include "fsm/machine_set.h"
#include "fsm/state.h"
#include "fsm/transition.h"
#include "log/llogger.h"
#include "util/timeutil.h"

using namespace kuafu {

StateMachine::StateMachine(const MachineType& type,
                           const std::string& name)
                           :MachineBase(type, name),
                           timeout_ms_(0) {
}

StateMachine::StateMachine(const StateMachine& rhs)
:MachineBase(rhs),
timeout_ms_(rhs.timeout_ms_), {
}

bool StateMachine::ForceState(const StateSharedPtr& state) {
    return InternalSetState<false>(state);
}

bool StateMachine::SetStartState(const State& state) {
    return InternalSetState<false>(state);
}

template<bool IsMetaState>
bool StateMachine::InternalSetState(const StateSharedPtr& state) {
    bool rt = false;

    auto found = std::find(states_.begin(),
                states_.end(),
                [](StateSharedPtr current) {
                return state.get() == current.get();
                });
    if (states_.end() != found) {
        if (IsMetaState) {
            meta_sate_ = state;;
        } else {
            current_state_ = state;;
        }
        rt = true;
    }

    return rt;
}

bool StateMachine::SetMetaState(const StateSharedPtr& state) {
    return InternalSetState<true>(state);
}

void StateMachine::ClearMetaState() {
    meta_state_.reset();
}

const StateSharedPtr StateMachine::GetCurrent() const {
   return current_state_;
}

void StateMachine::SetTimeout(unsigned long long timeout_ms) {
      timeout_ms_ = timeout_ms != 0 ? TimeUtil::GetTimeMs() + timeoutMS : 0;
}

unsigned long long StateMachine::GetTimeout() const {
   return timeout_ms_;
}

bool StateMachine::IsTimeout() const {
   bool rt = false;

   if (timeout_ms_ != 0) {
      rt = (TimeUtil::GetTimeMs() > timeout_ms_);
   }

   return rt;
}

bool StateMachine::ProcessNormalStateTransition(Event* event)
{
   if (mCurrent)
   {
      Transition* transition = NULL;
      for (vector<Transition*>::iterator i = mCurrent->mTransitions.begin();
         i != mCurrent->mTransitions.end(); ++i)
      {
         transition = *i;
         if (transition->isMatch(event, *this))
         {
            EInfoLog("Event (match) type: " << event->toString().c_str() << ", Type: " << mType.getName().c_str() << ", Name: " << mName.c_str());

            if (transition->getTransitionType() == Transition::tt_NormalTransition)
            {
               EDebugLog("Entering Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               try
               {
                  mCurrent->OnExit(*this, *mCurrent);
                  EDebugLog("Exited Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               }
               catch (...)
               {
                  EErrLog("Caught exception at Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               }
            }

            if (transition->getTransitionType() == Transition::tt_NormalTransition)
            {
               EInfoLog("Normal Transition from: " << mCurrent->getName()
                  << " -> " << transition->mTo->getName()
                  << ", for Machine"
                  << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               EDebugLog("Transition: ["
                  << transition->getName()
                  << "]");
            }
            else
            {
               EInfoLog("Internal Transition for state: " << mCurrent->getName()
                  << ", for Machine"
                  << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               EDebugLog("Transition: ["
                  << transition->getName()
                  << "]");
            }

            try
            {
               transition->OnTransition(
                  *this,
                  *mCurrent,
                  *transition,
                  event,
                  *(transition->mTo));
               if (transition->getTransitionType() == Transition::tt_NormalTransition)
               {
                  EDebugLog("Exited Normal Transition: ["
                     << transition->getName()
                     << "] from: " << mCurrent->getName()
                     << " -> " << transition->mTo->getName()
                     << ", for Machine"
                     << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               }
               else
               {
                  EDebugLog("Exited Internal Transition: ["
                     << transition->getName()
                     << "] for state: " << mCurrent->getName()
                     << ", for Machine"
                     << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               }
            }
            catch (...)
            {
               if (transition->getTransitionType() == Transition::tt_NormalTransition)
               {
                  EErrLog("Caught exception at Normal Transition action: ["
                     << transition->getName()
                     << "] from: " << mCurrent->getName()
                     << " -> " << transition->mTo->getName()
                     << ", for Machine"
                     << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               }
               else
               {
                  EErrLog("Caught exception at Internal Transition action: ["
                     << transition->getName()
                     << "] for state: " << mCurrent->getName()
                     << ", for Machine"
                     << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
               }
#ifndef BOOST_NO_EXCEPTIONS
               throw;
#endif
            }
            mPrevious = mCurrent;
            mCurrent = transition->mTo;

            if (transition->getTransitionType() == Transition::tt_NormalTransition)
            {
               // reset timer only on entry
               if (event->mMachineSet)
               {
                  event->mMachineSet->updateTimeoutMahcine(this, mCurrent->getTimeout());
                  setTimeout(mCurrent->getTimeout());
               }
               EDebugLog("Entering Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << transition->mTo->getName());
               try
               {
                  mCurrent->OnEnter(*this, *mCurrent);
                  EDebugLog("Exited Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << transition->mTo->getName());
               }
               catch (...)
               {
                  EErrLog("Caught exception at Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
#ifndef BOOST_NO_EXCEPTIONS
                  throw;
#endif
               }
            }
            return true;
         }
         //else
         //{
         //   EDebugLog("For Transition: " << transition->getName() << "| Event (nomatch) type: " << *event << " (" << mType.getName() << " " << mName  << ")");
         //}
      }
   }
   return false;
}

bool
StateMachine::processMetaStateTransition(Event* event)
{
   // check meta transitions AFTER specific transitions
   if (mMetaState && mCurrent)
   {
      for (vector<Transition*>::const_iterator i = mMetaState->mTransitions.begin();
         i != mMetaState->mTransitions.end(); ++i)
      {
         Transition* transition = *i;
         if (transition->isMatch(event, *this))
         {
            if (transition->getTransitionType() == Transition::tt_NormalTransition)
            {
               EDebugLog("Entering Meta Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               try
               {
                  mCurrent->OnExit(*this, *mCurrent);
                  EDebugLog("Exited Meta Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               }
               catch (...)
               {
                  EErrLog("Caught exception at Meta Exit action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
#ifndef BOOST_NO_EXCEPTIONS
                  throw;
#endif
               }
            }

            EInfoLog( "Meta Transition from: Empty -> " << transition->mTo->getName()
               << ", for Machine"
               << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
            EDebugLog( "Meta Transition: [" << transition->getName()
               << "]");
            try
            {
               transition->OnTransition(
                  *this,
                  *mCurrent,
                  *transition,
                  event,
                  *(transition->mTo));
               EDebugLog( "Exited Meta Transition: [" << transition->getName()
                  << "] from: Empty -> " << transition->mTo->getName()
                  << ", for Machine"
                  << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
            }
            catch (...)
            {
               EErrLog( "Caught exception at Meta Transition action: [" << transition->getName()
                  << "] from: Empty -> " << transition->mTo->getName()
                  << ", for Machine"
                  << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
#ifndef BOOST_NO_EXCEPTIONS
               throw;
#endif
            }

            //State* previous = mCurrent;
            mCurrent = transition->mTo;

            // transitions to meta-state do not change the current state
            if (transition->getTransitionType() == Transition::tt_NormalTransition)
            {
               // reset timer only on entry
               if (event->mMachineSet)
               {
                  event->mMachineSet->updateTimeoutMahcine(this, (*i)->mTo->getTimeout());
                  setTimeout((*i)->mTo->getTimeout());
               }

               EDebugLog("Entering Meta Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               try
               {
                  mCurrent->OnEnter(*this, *(mCurrent));
                  EDebugLog("Exited Meta Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
               }
               catch (...)
               {
                  EErrLog("Caught exception at Meta Enter action: (" << mType.getName().c_str() << " " << mName.c_str() << ") " << mCurrent->getName());
#ifndef BOOST_NO_EXCEPTIONS
                  throw;
#endif
               }
            }
            return true;
         }
      }
   }
   return false;
}


bool
StateMachine::process(Event* event)
{
   EDebugLog("Process StateMachine name: " << getName().c_str()<< "| Machine type:"
      <<  getType().getName().c_str() << "| Current State: " << (mCurrent?mCurrent->getName():"nil"));
   if (processNormalStateTransition(event))
   {
      return true;
   }
   return processMetaStateTransition(event);
}

//ostream&
//StateMachine::output(ostream& str) const
//{
//   str << mType.getName() << "(" << mName << ")[" << std::endl;
//   if (mMetaState)
//   {
//      str << "!!" << mMetaState->getName() << std::endl;
//   }
//
//   for (StateListType::const_iterator s = mStates.begin();
//      s != mStates.end(); ++s)
//   {
//      for (vector<Transition*>::const_iterator t = (*s)->mTransitions.begin();
//         t != (*s)->mTransitions.end(); ++t)
//      {
//         if ((*s) == mCurrent)
//         {
//            str << "*";
//         }
//         str << (*s)->getName() << " -" << (*t)->getName() << "-> " << (*t)->mTo->getName() << std::endl;
//      }
//   }
//   str << "]";
//
//   return str;
//}

//ostream&
//LUtil::operator<<(ostream& strm, const Machine& machine)
//{
//   return machine.output(strm);
//}

//void 
//ActionMachine::addNonTransitiveAction(NonTransitiveAction& action)
//{
//   ActionListType::iterator found = 
//      find(mNonTransitiveActions.begin(), mNonTransitiveActions.end(), &action);
//
//   assert(found == mNonTransitiveActions.end());
//   if (found == mNonTransitiveActions.end())
//   {
//      mNonTransitiveActions.push_back(&action);
//   }
//}


bool
ActionMachine::process(Event* event)
{
   EDebugLog("Process ActionMachine name: " << getName().c_str() << "| Machine type:"
      <<  getType().getName().c_str());
   for(ActionListType::const_iterator i = mNonTransitiveActions.begin();
      i != mNonTransitiveActions.end(); ++i)
   {
      NonTransitiveAction* nonTransitiveTransition = *i;
      if(nonTransitiveTransition->isMatch(event, *this))
      {
         EInfoLog("Non-transitive action: " << nonTransitiveTransition->getName()
            << " for "
            << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
         try
         {
            nonTransitiveTransition->OnAction(
               *this,
               *nonTransitiveTransition,
               event);
            EDebugLog("Exited Non-transitive action: " << nonTransitiveTransition->getName()
               << " for "
               << " (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
         }
         catch (...)
         {
            EErrLog("Caught exception at Non-transitive action: " << nonTransitiveTransition->getName()
               << " for (" << mType.getName().c_str() << " " << mName.c_str() << ") ");
#ifndef BOOST_NO_EXCEPTIONS
            throw;
#endif
         }
         return true;
      }
   }
   return false;
}

}//namespace kuafu
