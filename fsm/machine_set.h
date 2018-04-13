#ifndef KUAFU_MACHINESET_H_
#define KUAFU_MACHINESET_H_

#include <vector>
#include <set>
#include <memory>

#include "lutil/util/Fifo.hxx"
#include "lutil/log/externallogger.hxx"

namespace kuafu 
{
class MachineSetHandler;
class MachineBase;
class MachineType;
class Event;

class MachineSet : public ExternalLogger {
   public:
      MachineSet();
      ~MachineSet();

   public:
      void registerHandler(std::shared_ptr<LUtil::MachineSetHandler> handler);
      // thread safe
      void enqueue(boost::shared_ptr<Event> event);

      void enqueueDirect(boost::shared_ptr<Event> event);

      bool hasEvents() const;

      void process();      
      void process(boost::shared_ptr<Event> event);      
      void processTimeoutMachine(const MachineBase* machine);
      
      // thread safe now!
      void addMachine(MachineBase* machine);
      void removeMachine(MachineBase* machine);
      MachineBase* getMachine(const MachineType& type, const string& name);
      void updateTimeoutMahcine(const MachineBase* machine, time_t timeout) ;

      bool hasHandler() const { return mEventHandler != NULL; }

   public: // events
      NotifyEvent OnProcessError;

   private: // methods
      bool processTargetMachineEvent(boost::shared_ptr<Event>& delEvent);
      bool processNoTargetMachineEvent(boost::shared_ptr<Event>& delEvent);

   private:
      typedef std::vector<LUtil::MachineBase*> MachinePtrList;
      MachinePtrList mMachines;
      typedef std::set<LUtil::MachineBase*> MachinePtrSet;
      MachinePtrSet mMachinesSet;
      Fifo<LUtil::Event> mFifo;
      boost::shared_ptr<LUtil::MachineSetHandler> mEventHandler;

      void* mOwnerThreadId;

      friend ostream& operator<<(ostream& strm, const MachineSet& ms);
};

ostream& operator<<(ostream& strm, const MachineSet& ms);
}  // namespace kuafu

#endif // #ifndef KUAFU_MACHINESET_H_
