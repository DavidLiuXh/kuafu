#ifndef LUTIL_TIMEOUTEVENT_HXX
#define LUTIL_TIMEOUTEVENT_HXX

#include <lutil/LUtil.h>
#include <lutil/fsm/Event.hxx>

#include <boost/scoped_ptr.hpp>

#ifdef USING_USTL
# include <ustl.h>
   using namespace ustl;
#else
# include <iostream>
# include <string>
   using namespace std;
#endif

namespace LUtil
{
   class LUTIL_API MachineSet;
   class LUTIL_API MachineType;

   class LUTIL_API TimeoutEvent : public Event
   {
   public:
      TimeoutEvent(MachineSet* machineSet,
         const MachineType& machineType,
         const string& machineName);

      const MachineType& getType() const;
      const string& getMachineName() const;

      virtual ostream& toStream(ostream&) const ;
      virtual std::string toString() const;

   private:
      boost::scoped_ptr<MachineType> mMachineType;
      string mMachineName ;
   };

} // namespace LUtil

#endif // #ifndef LUTIL_TIMEOUTEVENT_HXX
