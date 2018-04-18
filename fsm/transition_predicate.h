#ifndef KUAFU_TRANSITIONPREDICATE_H_
#define KUAFU_TRANSITIONPREDICATE_H_

#include "fsm/fsm_type.h"

namespace kuafu {

class IPredicate {
 public:
     virtual ~IPredicate() {
     }

     virtual bool operator() (const EventSharedPtr&,
                 const MachineBaseSharedPtr& machine) = 0;
};

template <typename EventType>
class SimplePredicate : public IPredicate {
 public:
     typedef typename EventType::Type EventEnum;

     SimplePredicate(EventEnum type)
         :type_(type) {
     }

     virtual bool operator() (const EventSharedPtr&,
                 const MachineBaseSharedPtr& machine) {
         const EventType* levent = dynamic_cast<const EventType*>(event);
         return (levent && levent->getType() == type_);
     }

 private:
   EventEnum type_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_TRANSITIONPREDICATE_H_
