#ifndef KUAFU_MACHINESET_H_
#define KUAFU_MACHINESET_H_

#include <vector>
#include <set>
#include <memory>
#include <thread>
#include <functional>
#include <iostream>
#include <atomic>

#include "util/fifo.h"
#include "log/externallogger.h"
#include "fsm/fsmtype.h"

namespace kuafu {

class MachineSetHandler;
class MachineBase;
class MachineType;
class Event;

class MachineSet : public ExternalLogger,
    public std::enable_shared_from_this<MachineSet> {
        friend std::ostream& operator<<(std::ostream& strm, const MachineSet& ms);
 public:
     static MachineSetSharedPtr MakeMachineSet();

 private:
     MachineSet();

 public:
     ~MachineSet();

 public:
      void RegisterHandler(MachineSetHandlerSharedPtr handler);

      // thread safe
      void Enqueue(EventSharedPtr event);

      void StartBackground(int sleep_ms);
      void StopBackground();

      void Process(); 
      void Process(EventSharedPtr event);
      void ProcessTimeoutMachine(MachineBaseSharedPtr machine);
      
      MachineBaseSharedPtr GetMachine(const MachineType& type, const std::string& name);
      void UpdateTimeoutMahcine(const MachineBase& machine, time_t timeout) ;

      bool HasHandler() const {
          return static_cast<bool>(event_handler_);
      }

   public:
      typedef std::function<void()> NotifyEvent;
      NotifyEvent OnProcessError;

   private:
      void AddMachine(MachineBaseSharedPtr machine);
      void RemoveMachine(MachineBaseSharedPtr machine);

      bool ProcessTargetMachineEvent(const EventSharedPtr& event);
      bool ProcessNoTargetMachineEvent(const EventSharedPtr& event);
      void InternalProcessTimeoutEvent();

   private:
      typedef std::vector<MachineBaseSharedPtr> MachinePtrList;
      typedef std::set<MachineBaseSharedPtr> MachinePtrSet;

      MachinePtrList machine_list_;
      MachinePtrSet machine_set_;

      Fifo<Event> event_fifo_;

      MachineSetHandlerSharedPtr event_handler_;

      std::thread::id owner_thread_id_;

      std::atomic<bool> background_thread_stop_flag_;
      std::unique_ptr<std::thread> background_thread_;
};

std::ostream& operator<<(std::ostream& strm, const MachineSet& ms);

}  // namespace kuafu

#endif // #ifndef KUAFU_MACHINESET_H_
