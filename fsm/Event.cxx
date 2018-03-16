#include "stdafx.h"

#include <lutil/fsm/Event.hxx>

using namespace LUtil;

Event::Event(const vector<MachineBase*>& targetMachines) throw()
:mTargetMachines(targetMachines)
,mMachineSet(0)
{
}

Event::Event(MachineBase* targetMachine) throw()
:mMachineSet(0)
{
   if (targetMachine)
   {
      mTargetMachines.push_back(targetMachine);
   }
}

Event::~Event()
{
}

ostream& 
LUtil::operator<<(ostream& strm, const Event& event)
{
   return event.toStream(strm);
}