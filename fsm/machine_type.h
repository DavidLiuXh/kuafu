#ifndef KUAFU_MACHINETYPE_H_
#define KUAFU_MACHINETYPE_H_

#include <string>

namespace kuafu {
class MachineType
{
 public:
     MachineType(const std::string& name);
     MachineType(const MachineType& machine_type);

     ~MachineType();

     const std::string& GetName() const {
         return name_;
     }
     void SetName(const std::string& name) {
         name_ = name;
     }

     bool operator==(const MachineType& rhs) const;
     bool operator!=(const MachineType& rhs) const;

private:
     int id_;
     std::string name_;
};

std::ostream& operator<<(std::ostream& str, const MachineType& type);

} // namespace LUtil

#endif // #ifndef LUTIL_MACHINETYPE_HXX
