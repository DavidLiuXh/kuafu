#ifndef KUAFU_ADDMACHINEEVENT_H_
#define KUAFU_ADDMACHINEEVENT_H_

#include <sstream>

#include "fsm/fsmtype.h"

namespace kuafu {

class MachineBase;

enum class MachineOperator {
    MO_ADD,
    MO_REMOVE,
};

static const char* [] kMachineOperatorTag = {
    "AddMachineEvent", //MO_ADD
    "RemoveMachineEvent" //MO_REMOVE
};

template<MachineOperator mo>
class MachineOperationEvent : public Event {
 public:
     MachineOperationEvent(const MahcineBaseSharedPtr& machine)
         :machine_(machine) {
         }
     ~MachineOperationEvent() {
     }

     MachineBase* GetMachine() const;

     virtual std::ostream& ToStream(std::ostream& str) const {
         str << static_cast<const char*>(kMachineOperatorTag[mo])
             << static_cast<const char*>("[")
             << static_cast<const char*>(" ") << machine_->GetName()
             << static_cast<const char*>("]");
         return str;
     }

     virtual std::string ToString() const {
         std::stringstream str;
         str << static_cast<const char*>(kMachineOperatorTag[mo])
             << static_cast<const char*>("[")
             << static_cast<const char*>(" ") << machine_->GetName()
             << static_cast<const char*>("]");
         return str.str();
     }

 private:
     MachineBaseWeakPtr machine_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_ADDMACHINEEVENT_H_
