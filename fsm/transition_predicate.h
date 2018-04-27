#ifndef KUAFU_TRANSITIONPREDICATE_H_
#define KUAFU_TRANSITIONPREDICATE_H_

#include <memory>

#include "fsm/fsmtype.h"

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

     virtual bool operator() (const EventSharedPtr& event,
                 const MachineBaseSharedPtr& machine) {
         auto levent = std::dynamic_pointer_cast<EventType>(event);
         return (levent && levent->GetType() == type_);
     }

 private:
   EventEnum type_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_TRANSITIONPREDICATE_H_
