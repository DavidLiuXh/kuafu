#ifndef KUAFU_TIMEOUTPREDICATE_H_
#define KUAFU_TIMEOUTPREDICATE_H_

#include "fsm/transition_predicate.h"
#include "fsm/machine_type.h"

namespace kuafu {

class TimeoutPredicate : public IPredicate {
 public:
     TimeoutPredicate(const MachineType& source_achine_type);

     virtual bool operator() (const EventSharedPtr& event,
                 const MachineBaseSharedPtr& machine);

 private:
     MachineType source_machine_type_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_TIMEOUTPREDICATE_H_
