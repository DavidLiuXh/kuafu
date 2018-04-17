#ifndef KUAFU_TIMEOUTEVENT_H_
#define KUAFU_TIMEOUTEVENT_H_

#include <sstream>

#include "fsm/event.h"
#include "fsm/fsmtype.h"
#include "fsm/machine_type.h"

namespace kuafu {

class  MachineSet;
class  MachineType;

class TimeoutEvent : public Event {
 public:
     TimeoutEvent(const MachineSetSharedPtr& machine_set,
                 const MachineType& machine_type,
                 const std::string& machine_name);

     const MachineType& GetType() const;
     const std::string& GetMachineName() const;

     virtual std::ostream& ToStream(std::ostream&) const;
     virtual std::string ToString() const;

 private:
     MachineType machine_type_;
     std::string machine_name_;
};

} //namespace kuafu

#endif // #ifndef KUAFU_TIMEOUTEVENT_H_
