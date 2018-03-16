#ifndef LUTIL_MACHINESET_HXX
#define LUTIL_MACHINESET_HXX

//#include <lutil/fsm/Event.hxx>
#include <lutil/util/Fifo.hxx>
#include <lutil/log/externallogger.hxx>

#include <vector>
#include <set>
#include <memory>

namespace LUtil
{
class LUTIL_API MachineSetHandler;
class LUTIL_API MachineBase;
class LUTIL_API MachineType;
class LUTIL_API Event;

class LUTIL_API MachineSet : public ExternalLogger
{
   public:
      MachineSet() throw();
      ~MachineSet();

      void registerHandler(boost::shared_ptr<LUtil::MachineSetHandler> handler);
      // thread safe
#if defined(_WIN32)
      __declspec(deprecated("Deprecated Warning: should use enqueue(boost::shared_ptr<Event>) instead")) 
#endif
      void enqueue(Event* event);
      void enqueue(boost::shared_ptr<Event> event);
      void enqueue(auto_ptr<Event> event);
#if defined(_WIN32)
      __declspec(deprecated("Deprecated Warning: should use enqueueDirect(boost::shared_ptr<Event>) instead")) 
#endif
      void enqueueDirect(Event* event);
      void enqueueDirect(boost::shared_ptr<Event> event);
      void enqueueDirect(auto_ptr<Event> event);
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

#if defined(_WIN32) && !defined(HAVE_LIBPTHREAD)
      DWORD mOwnerThreadId;
#else
      void* mOwnerThreadId;
#endif

      friend ostream& operator<<(ostream& strm, const MachineSet& ms);
};

ostream& operator<<(ostream& strm, const MachineSet& ms);
}  // namespace LUtil

#endif // #ifndef LUTIL_MACHINESET_HXX
