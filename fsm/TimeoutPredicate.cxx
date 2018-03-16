#include "stdafx.h"

#include <lutil/fsm/TimeoutPredicate.hxx>

#include <lutil/fsm/TimeoutEvent.hxx>
#include <lutil/fsm/Machine.hxx>

using namespace LUtil;

TimeoutPredicate::TimeoutPredicate(const MachineType& sourceMachineType)
   : mSourceMachineType(new (nothrow) MachineType(sourceMachineType))
{
}

bool
TimeoutPredicate::operator()(const Event* event,
                             const MachineBase& machine) throw()
{
   const TimeoutEvent* tevent = dynamic_cast<const TimeoutEvent*>(event);
   if (!tevent)
   {
      return false;
   }

   if (*mSourceMachineType != tevent->getType())
   {
      return false;
   }

   return machine.getName() == tevent->getMachineName();
}