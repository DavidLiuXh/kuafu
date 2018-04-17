#include "fsm/machine_type.h"

#include <memory>
#include <atomic>

namespace kuafu {

static std::atomic<int> kMachineTypeIdGenarator(0);

MachineType::MachineType(const std::string& name)
    :id_(++kMachineTypeIdGenarator),
    name_(name) {
}

MachineType::MachineType(const MachineType& machine_type)
:id_(machine_type.id_)
,name_(machine_type.name_) {
}

MachineType::~MachineType() {
}

bool MachineType::operator==(const MachineType& rhs) const {
   return id_ == rhs.id_;
}

bool MachineType::operator!=(const MachineType& rhs) const {
   return id_ != rhs.id_;
}

std::ostream& operator<<(std::ostream& strm, const MachineType& machineType) {
   strm << machineType.GetName();
   return strm;
}

} //namespace kuafu
