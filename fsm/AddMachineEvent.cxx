#include "stdafx.h"


#include <lutil/fsm/AddMachineEvent.hxx>
#include <lutil/fsm/Machine.hxx>

#include <sstream>

using namespace LUtil;

AddMachineEvent::AddMachineEvent(MachineBase* machine)
   : mMachine(machine)
{
}

AddMachineEvent::~AddMachineEvent()
{
}

MachineBase* 
AddMachineEvent::getMachine() const
{
   return mMachine;
}

ostream& 
AddMachineEvent::toStream(ostream& str) const
{
   str << static_cast<const char*>("AddMachineEvent[")
       << static_cast<const char*>(" ") << mMachine->getName()
       << static_cast<const char*>("]");
   return str;
}

std::string AddMachineEvent::toString() const
{
   std::stringstream str;
   str << static_cast<const char*>("AddMachineEvent[")
      << static_cast<const char*>(" ") << mMachine->getName()
      << static_cast<const char*>("]");
   return str.str();
}

RemoveMachineEvent::RemoveMachineEvent(MachineBase* machine)
   : mMachine(machine)
{
}

RemoveMachineEvent::~RemoveMachineEvent()
{
}

MachineBase*
RemoveMachineEvent::getMachine() const
{
   return mMachine;
}


//virtual
ostream& 
RemoveMachineEvent::toStream(ostream& str) const
{
   str << static_cast<const char*>("RemoveMachineEvent[")
       << static_cast<const char*>(" ") << mMachine->getName()
       << static_cast<const char*>("]");
   return str;
}

std::string RemoveMachineEvent::toString() const
{
   std::stringstream str;
   str << static_cast<const char*>("AddMachineEvent[")
      << static_cast<const char*>(" ") << mMachine->getName()
      << static_cast<const char*>("]");
   return str.str();
}

OwnerRemoveMachineEvent::OwnerRemoveMachineEvent(boost::shared_ptr<MachineBase> machine)
:RemoveMachineEvent(machine.get())
,mOwnedMachine(machine)
{

}
