#ifndef KUAFU_FSMTYPE_H_
#define KUAFU_FSMTYPE_H_

#include <memory>
#include <vector>

namespace kuafu {

class MachineBase;
class MachineSet;
class MachineSetHandler;
class Event;

typedef std::shared_ptr<MachineBase> MachineBaseSharedPtr;
typedef std::weak_ptr<MachineBase> MachineBaseWeakPtr;
typedef std::vector<MachineBaseWeakPtr> MachineBaseWeakPtrVec;

typedef std::shared_ptr<MachineSet> MachineSetSharedPtr;
typedef std::weak_ptr<MachineSet> MachineSetWeakPtr;

typedef std::shared_ptr<MachineSetHandler> MachineSetHandlerSharedPtr;

typedef std::shared_ptr<Event> EventSharedPtr;

} //namespace kuafu

#endif //#ifndef KUAFU_FSMTYPE_H_
