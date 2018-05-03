#ifndef KUAFU_ADDMACHINEEVENT_H_
#define KUAFU_ADDMACHINEEVENT_H_

#include <sstream>

#include "fsm/event.h"
#include "fsm/fsmtype.h"
#include "fsm/machine.h"

namespace kuafu {

class MachineBase;

enum class MachineOperator {
    MO_ADD,
    MO_REMOVE,
};

class MachineOperationEvent : public EventTemplate<MachineOperator> {
 public:
     MachineOperationEvent(MachineOperator type, const MachineBaseSharedPtr& machine)
         :EventTemplate<MachineOperator>(type)
         ,machine_(machine) {
         }
     ~MachineOperationEvent() {
     }

     MachineBaseSharedPtr GetMachine() const {
         return machine_;
     }

     virtual std::ostream& ToStream(std::ostream& str) const {
         str << static_cast<const char*>(type_ == MachineOperator::MO_ADD ? "AddMachineEvent" : "RemoveMachineEvent")
             << static_cast<const char*>("[")
             << static_cast<const char*>(" ") << machine_->GetName()
             << static_cast<const char*>("]");
         return str;
     }

     virtual std::string ToString() const {
         std::stringstream str;
         str << static_cast<const char*>(type_ == MachineOperator::MO_ADD ? "AddMachineEvent" : "RemoveMachineEvent")
             << static_cast<const char*>("[")
             << static_cast<const char*>(" ") << machine_->GetName()
             << static_cast<const char*>("]");
         return str.str();
     }

 private:
     MachineBaseSharedPtr machine_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_ADDMACHINEEVENT_H_
