#include "fsm/event.h"

namespace kuafu {

Event::Event(const MachineBaseWeakPtrVec& target_machines)
:target_machines_(std::move(target_machines)) {
}

Event::Event(const MachineBaseSharedPtr& target_machine) {
   if (target_machine) {
      target_machines_.emplace_back(target_machine);
   }
}

Event::Event(const MachineSetSharedPtr& machine_set)
:machine_set_(machine_set) {
}

std::ostream& operator<<(std::ostream& strm, const Event& event) {
   return event.ToStream(strm);
}

} //namespace kuafu
