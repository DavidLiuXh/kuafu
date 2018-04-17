#ifndef KUAFU_EVENT_H_
#define KUAFU_EVENT_H_

#include <memory>
#include <ostream>

#include "fsm/fsmtype.h"

namespace kuafu {

class MachineBase;
class MachineSet;

class Event {
  friend class MachineSet;
  friend class StateMachine;
  friend class ActionMachine;

public:
  Event(const MachineBaseWeakPtrVec& targetMachines);
  Event(const MachineBaseSharedPtr& targetMachine);
  Event(const MachineSetSharedPtr& machineSet);

  virtual ~Event() {
  }

  const MachineSet* GetMachineSet() const {
      MachineSetSharedPtr machine_set = machine_set_.lock();
      return machine_set.get();
  }

  MachineSet* GetMachineSet() {
      MachineSetSharedPtr machine_set = machine_set_.lock();
      return machine_set.get();
  }

public:
  virtual std::ostream& ToStream(std::ostream& str) const = 0;
  virtual std::string ToString() const = 0;

private:
  MachineBaseWeakPtrVec target_machines_;
  MachineSetWeakPtr machine_set_;
};

std::ostream& operator<<(std::ostream& str, const Event& event);

} //namespace kuafu

#endif // #ifndef KUAFU_EVENT_H_
