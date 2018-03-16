#include "stdafx.h"

#include <lutil/fsm/MachineSetHandler.hxx>

//#include <lutil/fsm/AddMachineEvent.hxx>
#include <lutil/fsm/Machine.hxx>
#include <lutil/fsm/MachineSet.hxx>
#include <lutil/fsm/TimeoutEvent.hxx>

using namespace LUtil;

MachineSetHandler::MachineSetHandler()
//:mMachineSet(0)
{}

MachineSetHandler::~MachineSetHandler()
{

}

void MachineSetHandler::processEvent(boost::shared_ptr<LUtil::Event>* eventSharedPtrPtr)
{
   assert(/*mMachineSet && */eventSharedPtrPtr);
   boost::shared_ptr<LUtil::Event> eventPtr(*eventSharedPtrPtr);
   delete eventSharedPtrPtr;
   assert(eventPtr->getMachineSet());
   eventPtr->getMachineSet()->process(eventPtr);
}

void MachineSetHandler::processTimeOutMachine(MachineSet* machineSet, const MachineBase* machine)
{
   assert(machineSet);
   machineSet->processTimeoutMachine(machine);
}
