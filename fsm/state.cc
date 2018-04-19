#include "stdafx.h"

#include <lutil/fsm/State.hxx>
#include <lutil/fsm/Machine.hxx>

#include <cassert>

using namespace LUtil;

State::State(StateMachine& owner, const char* name, time_t timeoutMS)
:mName(name)
,mTimeoutMS(timeoutMS)
{
   owner.mStates.push_back(this);
}

State::State(StateMachine& owner, const State& copy)
:mName(copy.mName)
,mTimeoutMS(copy.mTimeoutMS)
{
   owner.mStates.push_back(this);
}

//virtual 
State::~State()
{
}

void
State::clearActions()
{
   OnEnter.Clear();
   OnExit.Clear();
}

time_t
State::getTimeout() const
{
   return mTimeoutMS;
}

void
State::setTimeout(time_t newTimeoutMS)
{
   mTimeoutMS = newTimeoutMS;
}

bool 
State::operator==(const State& rhs) const
{
   return this == &rhs;
}

bool 
State::operator!=(const State& rhs) const
{
   return this != &rhs;
}