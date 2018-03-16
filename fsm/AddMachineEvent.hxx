#ifndef LUTIL_ADDMACHINEEVENT_HXX
#define LUTIL_ADDMACHINEEVENT_HXX

#include <lutil/LUtil.h>
#include <lutil/fsm/Event.hxx>

#include <boost/shared_ptr.hpp>

#ifdef USING_USTL
# include <ustl.h>
using namespace ustl;
#else
# include <iostream>
using namespace std;
#endif

namespace LUtil
{
   class LUTIL_API MachineBase;
   class LUTIL_API AddMachineEvent : public Event
   {
   public:
      AddMachineEvent(MachineBase* machine);
      ~AddMachineEvent();

      MachineBase* getMachine() const;

      virtual ostream& toStream(ostream& str) const;

      virtual std::string toString() const;

   private:
      MachineBase* mMachine;
   };

   class LUTIL_API RemoveMachineEvent : public Event
   {
   public:
      RemoveMachineEvent(MachineBase* machine);
      ~RemoveMachineEvent();

      MachineBase* getMachine() const;

      virtual ostream& toStream(ostream& str) const;
      virtual std::string toString() const;

   private:
      MachineBase* mMachine;
   };

   class LUTIL_API OwnerRemoveMachineEvent : public RemoveMachineEvent
   {
   public:
      OwnerRemoveMachineEvent(boost::shared_ptr<MachineBase> machine);

   private:
      boost::shared_ptr<MachineBase> mOwnedMachine;
   };

} // namespace LUtil

#endif // #ifndef LUTIL_ADDMACHINEEVENT_HXX
