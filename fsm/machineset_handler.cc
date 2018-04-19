#include "fsm/machineset_handler.h"
#include "fsm/machine_set.h"
#include "fsm/machine.h"
#include "fsm/timeout_event.h"

namespace kuafu {

void MachineSetHandler::processEvent(const EventSharedPtr& event) {
    if (event) {
        MachineSetSharedPtr&& machine_set = event->GetMachineSet();
        if (machine_set) {
            machine_set->process(event);
        }
    }
}

void MachineSetHandler::processTimeOutMachine(const MachineSetSharedPtr& machine_set,
            const MachineBaseSharedPtr& machine) {
    if (machine_set) {
        machine_set->processTimeoutMachine(machine);
    }
}

} //namespace kuafu
