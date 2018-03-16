#include "stdafx.h"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <sstream>

#include <boost/config.hpp>

#include <lutil/fsm/Event.hxx>
#include <lutil/fsm/Machine.hxx>
#include <lutil/fsm/MachineSet.hxx>
#include <lutil/fsm/State.hxx>
#include <lutil/fsm/Transition.hxx>
#include <lutil/util/TimeUtil.hxx>
#include <lutil/log/LLogger.hxx>

using namespace LUtil;

StateMachine::StateMachine(const MachineType& type,
                           const string& name)
                           : MachineBase(type, name),
                           mTimeoutMS(0),
                           mCurrent(NULL),
                           mPrevious(NULL),
                           mMetaState(NULL)
{
}

StateMachine::StateMachine(const StateMachine& rhs)
:MachineBase(rhs),
mTimeoutMS(rhs.mTimeoutMS),
mCurrent(NULL/*rhs.mCurrent*/),
mPrevious(NULL/*rhs.mPrevious*/),
//mStates(rhs.mStates),
mMetaState(NULL/*rhs.mMetaState*/)
{
}

bool
StateMachine::forceState(State& state)
{
   bool rt = false;

   if (mStates.end() != find(mStates.begin(), mStates.end(), &state))
   {
      mCurrent = &state;
      rt = true;
   }
   else
   {
      assert(false);
   }

   return rt;
}

bool
StateMachine::setStartState(State& state)
{
   bool rt = false;

   assert(mCurrent == 0);
   if (find(mStates.begin(), mStates.end(), &state) != mStates.end())
   {
      mCurrent = &state;
      rt = true;
   }
   else
   {
      assert(false);
   }

   return rt;
}

bool
StateMachine::setMetaState(State& state)
{
   bool rt = false;

   if (mStates.end() != find(mStates.begin(), mStates.end(), &state))
   {
      mMetaState = &state;
      rt = true;
   }
   else
   {
      assert(false);
   }

   return rt;
}

void
StateMachine::clearMetaState()
{
   mMetaState = 0;
}

const State*
StateMachine::getCurrent() const
{
   return mCurrent;
}

void
StateMachine::setTimeout(unsigned long long timeoutMS)
{
   if (timeoutMS != 0)
   {
      mTimeoutMS = TimeUtil::getTimeMs() + timeoutMS;
   }
   else if (mTimeoutMS)
   {
      mTimeoutMS = 0;
   }
}

unsigned long long
StateMachine::getTimeout() const
{
   return mTimeoutMS;
}

bool
StateMachine::isTimeout() const
{
   bool rt = false;

   if (mTimeoutMS != 0)
   {
      rt = (TimeUtil::getTimeMs() > mTimeoutMS);
   }

   return rt;
}

bool
StateMachine::processNormalStateTransition(Event* event)
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
#ifndef BOOST_NO_EXCEPTIONS
                  throw;
#endif
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

