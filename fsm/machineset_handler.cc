#include "fsm/machineset_handler.h"

#include "fsm/machine_set.h"
#include "fsm/machine.h"
#include "fsm/event.h"
#include "fsm/timeout_event.h"

namespace kuafu {

void MachineSetHandler::ProcessEvent(const EventSharedPtr& event) {
    if (event) {
        const MachineSetSharedPtr machine_set = event->GetMachineSet();
        if (machine_set) {
            machine_set->Process(event);
        }
    }
}

void MachineSetHandler::ProcessTimeOutMachine(const MachineSetSharedPtr& machine_set,
            const MachineBaseSharedPtr& machine) {
    if (machine_set) {
        machine_set->ProcessTimeoutMachine(machine);
    }
}

} //namespace kuafu
