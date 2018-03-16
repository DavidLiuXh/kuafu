#ifndef LUTIL_MACHINESETHANDLER_HXX
#define LUTIL_MACHINESETHANDLER_HXX

#include <lutil/LUtil.h>

#include <boost/shared_ptr.hpp>

namespace LUtil
{

   class LUTIL_API Event;
   class LUTIL_API MachineSet;
   class LUTIL_API MachineBase;
   class LUTIL_API MachineSetHandler
   {
   public:
      enum
      {
         MS_Skip, // if skip don't call MachineSet's function
         MS_Continue // if continue ,call MachineSet's function
      };

   public:
      MachineSetHandler();
      virtual ~MachineSetHandler();

      //virtual void updateMachineSet(MachineSet* machineSet);
      virtual int onEventEnqueue(boost::shared_ptr<Event> event) = 0 ;
      void processEvent(boost::shared_ptr<LUtil::Event>* eventSharedPtrPtr);

      virtual int onUpdateMachineTimeOut(MachineSet* machineSet, const MachineBase* machine, time_t seconds) = 0;
      void processTimeOutMachine(MachineSet* machineSet, const MachineBase* machine);
   protected:
      //MachineSet* mMachineSet;
   };

   class LUTIL_API IMachineSetExternalHandler
   {
   public:
      virtual ~IMachineSetExternalHandler() { }

      virtual void registerHandler(boost::shared_ptr<LUtil::MachineSetHandler> handler) = 0;
   };

} // namespace LUtil

#endif // #ifndef LUTIL_MACHINESETHANDLER_HXX
