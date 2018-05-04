#include "fsm/machine_set.h"

#include <cassert>
#include <typeinfo>
#include <sstream>
#include <memory>
#include <exception>

#include "fsm/machineset_handler.h"
#include "fsm/machine.h"
#include "fsm/event.h"
#include "fsm/add_machine_event.h"
#include "fsm/timeout_event.h"
#include "log/kflogger.h"

namespace kuafu {

MachineSetSharedPtr MachineSet::MakeMachineSet() {
    return MachineSetSharedPtr(new MachineSet());
}

MachineSet::MachineSet()
:owner_thread_id_(std::this_thread::get_id()) {
}

MachineSet::~MachineSet() {
   event_fifo_.Clear();
}

void MachineSet::RegisterHandler(MachineSetHandlerSharedPtr handler) {
   event_handler_ = handler;
}

void MachineSet::AddMachine(MachineBaseSharedPtr machine) {
   ExternalDebugLog("Adding machine: " << machine->GetName()
               << "(" << machine->GetType().GetName() << ")");

   if (machine_set_.insert(machine).second) {
       //machine->Birth();

       machine_list_.push_back(machine);

       machine->level = level;
       machine->debuglog_delegate = debuglog_delegate;
       machine->infolog_delegate = infolog_delegate;
       machine->warninglog_delegate = warninglog_delegate;
       machine->errlog_delegate = errlog_delegate;
   }
}

void MachineSet::RemoveMachine(MachineBaseSharedPtr machine) {
   ExternalDebugLog("Removing machine: " << machine->GetName()
               << "(" << machine->GetType().GetName() << ")");

   if (machine_set_.erase(machine)) {
      for (auto i = machine_list_.begin();
         i != machine_list_.end(); ++i) {
         if (i->get() == machine.get()) {
             machine_list_.erase(i);
             ExternalDebugLog("Removing machine: " << machine->GetName() << " OK!");
             return;
         }
      }
   }
}

MachineBaseSharedPtr MachineSet::GetMachine(const MachineType& type, const std::string& name) {
    MachineBaseSharedPtr found = nullptr;

    for (auto i = machine_list_.begin(), e = machine_list_.end();
                i != e; ++i) {
        if ((*i)->GetType() == type &&
                    (*i)->GetName() == name) {
            found = *i;
            break;
        }
    }

    return found;
}

void MachineSet::Enqueue(EventSharedPtr event) {
   ExternalInfoLog("enqueue " << event->ToString());

   event->machine_set_ = shared_from_this();
   if (event_handler_ &&
               event_handler_->OnEventEnqueue(event) == MachineSetHandler::HandleResult::HR_SKIP) {
       ExternalDebugLog("use machine set handler");
   } else {
       event_fifo_.Add(event);
   }
}

void MachineSet::Process() {
   while (event_fifo_.MessageAvailable()) {
      Process(event_fifo_.GetNext());
   }

   if (!event_handler_) {
      for (auto i = machine_list_.begin(), e = machine_list_.end();
         i != e; ++i) {
         if ((*i)->IsTimeout()) {
            (*i)->SetTimeout(0);
            std::shared_ptr<TimeoutEvent> timeout_event = std::make_shared<TimeoutEvent>(
                        shared_from_this(),
                        (*i)->GetType(),
                        (*i)->GetName());
            (*i)->Process(timeout_event);
         }
      }
   }
}

bool MachineSet::ProcessTargetMachineEvent(const EventSharedPtr& event) {
    bool handled = false;

    for (auto i = event->target_machines_.begin(), e = event->target_machines_.end();
                i != e; ++i) {
        auto machine = i->lock();
        if (machine) {
            auto found = machine_set_.find(machine);
            if (found != machine_set_.end()) {
                ExternalDebugLog(__FUNCTION__ << " | target machine found: " << (*found)->GetName());
                handled |= (*found)->Process(event);
            }
        }
    }

    return handled;
}

bool MachineSet::ProcessNoTargetMachineEvent(const EventSharedPtr& event) {
   bool handled = false;

   for (auto i = machine_list_.rbegin(), e = machine_list_.rend();
      i != e; ++i) {
      handled |= (*i)->Process(event);
   }

   return handled;
}

void MachineSet::Process(EventSharedPtr event) {
    try {
        auto operate_machine_event = std::dynamic_pointer_cast<MachineOperationEvent>(event);
        if (operate_machine_event) {
            if (MachineOperator::MO_ADD == operate_machine_event->GetType()) {
                AddMachine(operate_machine_event->GetMachine());
            } else if (MachineOperator::MO_REMOVE == operate_machine_event->GetType()) {
                RemoveMachine(operate_machine_event->GetMachine());
            }

            return;
        }

        ExternalInfoLog("Handling event: " << event->ToString());

        bool handled = false;

        if (!event->target_machines_.empty()) {
            handled = ProcessTargetMachineEvent(event);
        } else {
            handled = ProcessNoTargetMachineEvent(event);
        }

        if (!handled) {
            ExternalErrorLog("Unhandled event: " << event->ToString());
        }
    } catch (std::exception& e) {
        ExternalErrorLog(__FUNCTION__ << " | caught exception: " << e.what());
        OnProcessError();
    } catch (...) {
        ExternalErrorLog(__FUNCTION__ << " | caught unknown exception");
        OnProcessError();
    }
}

void MachineSet::ProcessTimeoutMachine(MachineBaseSharedPtr machine) {
   auto found = machine_set_.find(machine);
   if (found != machine_set_.end()) {
      auto state_machine = std::dynamic_pointer_cast<StateMachine>(*found);
      if (state_machine) {
          std::shared_ptr<TimeoutEvent> timeout_event = std::make_shared<TimeoutEvent>(
                      shared_from_this(),
                      state_machine->GetType(),
                      state_machine->GetName());
         state_machine->SetTimeout(0);
         try {
            state_machine->Process(timeout_event);
         } catch (std::exception& e) {
            ExternalErrorLog(__FUNCTION__ << " | caught exception: " << e.what());
            OnProcessError();
         } catch (...) {
            ExternalErrorLog(__FUNCTION__ << " | caught unknown exception");
            OnProcessError();
         }
      }
   }
}

void MachineSet::UpdateTimeoutMahcine(const MachineBase& machine, time_t seconds_ms) {
   if (event_handler_ && seconds_ms > 0) {
      event_handler_->OnUpdateMachineTimeOut(shared_from_this(), machine, seconds_ms);
   }
}

std::ostream& operator<<(std::ostream& strm, const MachineSet& ms) {
   strm << static_cast<const char*>("Machines: ");
   for (auto i = ms.machine_list_.begin(), e = ms.machine_list_.end();
               i != e; ++i) {
      strm << (*i)->GetName()
          << static_cast<const char*>("(")
          << (*i)->GetType().GetName()
          << static_cast<const char*>("),");
   }

   return strm;
}
} //namespace kuafu
