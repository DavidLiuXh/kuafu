#ifndef KUAFU_MACHINESETHANDLER_H_
#define KUAFU_MACHINESETHANDLER_H_

#include "fsm/fsmtype.h"

namespace kuafu {

class Event;
class MachineSet;
class MachineBase;

class MachineSetHandler {
 public:
     enum class HandleResult {
         HR_SKIP, // if skip don't call MachineSet's function
         HR_CONTINUE // if continue ,call MachineSet's function
     };

 public:
     MachineSetHandler() {
     }

     virtual ~MachineSetHandler() {
     }

     void ProcessEvent(const EventSharedPtr& event);
     void ProcessTimeOutMachine(const MachineSetSharedPtr& machine_set,
                 const MachineBaseSharedPtr& machine);

 public:
     virtual HandleResult OnEventEnqueue(const EventSharedPtr& event) = 0 ;
     virtual int OnUpdateMachineTimeOut(const MachineSetSharedPtr& machine_set,
                 const MachineBase& machine,
                 time_t seconds) = 0;
};

class IMachineSetExternalHandler {
 public:
     virtual ~IMachineSetExternalHandler() {
     }

     virtual void RegisterHandler(const MachineSetHandlerSharedPtr& handler) = 0;
};

} // namespace kuafu

#endif // #ifndef KUAFU_MACHINESETHANDLER_H_
