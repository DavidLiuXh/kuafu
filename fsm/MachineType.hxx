#ifndef LUTIL_MACHINETYPE_HXX
#define LUTIL_MACHINETYPE_HXX

#include <lutil/LUtil.h>

#ifdef USING_USTL
#include <ustl.h>
   using namespace ustl;
#else
#include <iostream>
#include <string>
   using namespace std;
#endif

namespace LUtil
{
   class LUTIL_API MachineType
   {
   public:
      MachineType(const string& name);
      ~MachineType();

      const string& getName() const { return mName; }
      void setName(const string& name) { mName = name; }

      bool operator==(const MachineType& rhs) const;
      bool operator!=(const MachineType& rhs) const;

   private:
      int mNumber;
      string mName;
   };
   ostream& operator<<(ostream& str, const MachineType& event);

} // namespace LUtil

#endif // #ifndef LUTIL_MACHINETYPE_HXX
