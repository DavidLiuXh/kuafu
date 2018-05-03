#ifndef KUAFU_EVENT_H_
#define KUAFU_EVENT_H_

#include <memory>
#include <ostream>
#include <type_traits>

#include "fsm/fsmtype.h"

namespace kuafu {

class MachineBase;
class MachineSet;

class Event {
  friend class MachineSet;
  friend class StateMachine;
  friend class ActionMachine;

 public:
  Event(const MachineSetSharedPtr& machine_set)
      :machine_set_(machine_set) {
      }

  virtual ~Event() {
  }

 public:
    virtual std::ostream& ToStream(std::ostream& str) const = 0;
    virtual std::string ToString() const = 0;

    virtual const MachineSetSharedPtr GetMachineSet() const {
        return machine_set_.lock();
    }

    MachineSetSharedPtr GetMachineSet() {
        return machine_set_.lock();
    }

 protected:
    MachineSetWeakPtr machine_set_;
    MachineBaseWeakPtrVec target_machines_;
};

template<class EventType,
    class = typename std::enable_if<std::is_enum<EventType>::value>::type>
class EventTemplate : public Event {
  friend class MachineSet;
  friend class StateMachine;
  friend class ActionMachine;

 public:
  typedef EventType Type;

  EventTemplate(Type type)
      :Event(MachineSetSharedPtr()),
       type_(type) {
      }

  EventTemplate(Type type, const MachineBaseWeakPtrVec& target_machines)
      :Event(MachineSetSharedPtr()),
       type_(type) {
          target_machines_ = std::move(target_machines);
      }

  EventTemplate(Type type, const MachineBaseSharedPtr& target_machine)
      :Event(MachineSetSharedPtr()),
       type_(type) {
      if (target_machine) {
          target_machines_.emplace_back(target_machine);
      }
  }

  EventTemplate(Type type, const MachineSetSharedPtr& machine_set)
      :Event(machine_set),
       type_(type) {
      }

  virtual ~EventTemplate() {
  }

  Type GetType() const {
      return type_;
  }

protected:
  Type type_;
};

std::ostream& operator<<(std::ostream& str, const Event& event);

} //namespace kuafu

#endif // #ifndef KUAFU_EVENT_H_
