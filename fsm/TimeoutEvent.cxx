#include "stdafx.h"

#include <lutil/fsm/TimeoutEvent.hxx>
#include <lutil/fsm/MachineType.hxx>

#include <cassert>
#include <sstream>

using namespace LUtil;

TimeoutEvent::TimeoutEvent(MachineSet* machineSet,
                           const MachineType& machineType,
                           const string& machineName)
                           :Event(machineSet)
                           ,mMachineType(new (nothrow) MachineType(machineType))
                           ,mMachineName(machineName)
{
  assert(machineSet);
}

const MachineType&
TimeoutEvent::getType() const
{
  return *mMachineType;
}

const string&
TimeoutEvent::getMachineName() const
{
  return mMachineName;
}

ostream& 
TimeoutEvent::toStream(ostream& str) const
{
  str << static_cast<const char*>("TimeoutEvent[") << mMachineType->getName()
    << static_cast<const char*>(", ") << mMachineName
    << static_cast<const char*>("]");

  return str;
}

std::string TimeoutEvent::toString() const
{
   std::stringstream str;
   str << static_cast<const char*>("TimeoutEvent[") << mMachineType->getName()
      << static_cast<const char*>(", ") << mMachineName
      << static_cast<const char*>("]");

   return str.str();
}