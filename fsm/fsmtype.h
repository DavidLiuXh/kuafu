#ifndef KUAFU_FSMTYPE_H_
#define KUAFU_FSMTYPE_H_

#include <memory>
#include <vector>

namespace kuafu {

class MachineBase;
class ActionMachine;
class MachineSet;
class MachineSetHandler;
class Event;
class IPredicate;
class State;
class Transition;
class NonTransitiveAction;

typedef std::shared_ptr<MachineBase> MachineBaseSharedPtr;
typedef std::weak_ptr<MachineBase> MachineBaseWeakPtr;
typedef std::vector<MachineBaseWeakPtr> MachineBaseWeakPtrVec;

typedef std::shared_ptr<MachineSet> MachineSetSharedPtr;
typedef std::weak_ptr<MachineSet> MachineSetWeakPtr;

typedef std::shared_ptr<MachineSetHandler> MachineSetHandlerSharedPtr;

typedef std::shared_ptr<Event> EventSharedPtr;

typedef std::shared_ptr<IPredicate> IpredicateSharedPtr;

typedef std::shared_ptr<State> StateSharedPtr;
typedef std::weak_ptr<State> StateWeakPtr;

typedef std::shared_ptr<Transition> TransitionSharedPtr;
typedef std::shared_ptr<NonTransitiveAction> NonTransitiveActionSharedPtr;

typedef std::shared_ptr<ActionMachine> ActionMachineSharedPtr;

} //namespace kuafu

#endif //#ifndef KUAFU_FSMTYPE_H_
