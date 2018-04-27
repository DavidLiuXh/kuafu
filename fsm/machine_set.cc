#include "fsm/machine_set.h"

#include <cassert>
#include <typeinfo>
#include <sstream>

#include "fsm/machineset_handler.h"

#include "fsm/add_machine_event.h"
#include "fsm/timeout_event.h"
#include "log/llOgger.h"

namespace kuafu {

MachineSetSharedPtr MachineSet::MakeMachineSet() {
    return std::make_shared<MachineSet>();
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
   EDebugLog("Adding machine: " << machine->GetName()
               << "(" << machine->GetType().GetName() << ")");

   if (machine_set_.insert(machine).second) {
      machine_list_.push_back(machine);

      machine->debuglog_delegate = debuglog_delegate;
      machine->infolog_delegate = infolog_delegate;
      machine->warninglog_delegate = warninglog_delegate;
      machine->errlog_delegate = errlog_delegate;
   }
}

void MachineSet::RemoveMachine(MachineBaseSharedPtr machine) {
   EDebugLog("Removing machine: " << machine->GetName()
               << "(" << machine->GetType().GetName() << ")");

   if (machine_set_.erase(machine)) {
      for (auto i = machine_list_.begin();
         i != machine_list_.end(); ++i) {
         if (i->get() == machine.get()) {
             machine_list_.erase(i);
             EDebugLog("Removing machine: " << machine->GetName() << " OK!");
             return;
         }
      }
   }
}

MachineBaseSharedPtr MachineSet::GetMachine(const MachineType& type, const string& name) {
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
   EInfoLog("enqueue " << event->toString());

   event->machine_set_ = std::shared_from_this();
   if (event_handler_ &&
               event_handler->onEventEnqueue(event) == MachineSetHandler::HandleResult::MS_SKIP) {
       EDebugLog("use machine set handler");
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
            std::shared_ptr<TimeoutEvent> timeout_event(std::shared_from_this(),
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
      i != event->target_machines_.end(); ++i) {
      auto found = machine_set_.find(*i);
      if (found != machine_set_.end()) {
         handled |= found->Process(event);
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
        auto add_machine_event = std::dynamic_pointer_cast<AddMachineEvent>(event);
        if (add_machine_evnet) {
            AddMachine(add_machine_event->GetMachine());
            return;
        } else {
            auto remove_machine_event = std::dynamic_pointer_cast<RemoveMachineEvent>(event);
            if (remove_machine_event) {
                RemoveMachine(remove_machine_event->GetMachine());
                return;
            }
        }

        EInfoLog("Handling event: " << event->toString());

        bool handled = false;

        if (!event->target_machines_.empty()) {
            handled = ProcessTargetMachineEvent(event);
        } else {
            handled = ProcessNoTargetMachineEvent(event);
        }

        if (!handled) {
            EErrLog("Unhandled event: " << event->toString().c_str());
        }
    } catch (exception& e) {
        EErrLog(__FUNCTION__ << " | caught exception: " << e.what());
        OnProcessError();
    } catch (...) {
        EErrLog(__FUNCTION__ << " | caught unknown exception");
        OnProcessError();
    }
}

void MachineSet::ProcessTimeoutMachine(MachineBaseSharedPtr machine) {
   auto found = machine_set_.find(machine);
   if (found != machine_set_.end()) {
      auto state_machine = std::dynamic_pointer_cast<StateMachine>(*found);
      if (state_machine) {
          std::shared_ptr<TimeoutEvent> timeout_event(std::shared_from_this(),
                      state_machine->GetType(),
                      state_machine->GetName());
         state_machine->SetTimeout(0);
         try {
            state_machine->Process(timeout_event);
         } catch (exception& e) {
            EErrLog(__FUNCTION__ << " | caught exception: " << e.what());
            OnProcessError();
         } catch (...) {
            EErrLog(__FUNCTION__ << " | caught unknown exception");
            OnProcessError();
         }
      }
   }
}

void MachineSet::UpdateTimeoutMahcine(MachineBaseSharedPtr machine, time_t seconds_ms) {
   if (event_handler_ && seconds_ms > 0) {
      event_handler_->OnUpdateMachineTimeOut(this, machine, seconds_ms);
   }
}

std::ostream& operator<<(std::ostream& strm, const MachineSet& ms) {
   strm << static_cast<const char*>("Machines: ");
   for (auto i = ms.machine_list_.begin(), e = ms.machine_list_.end();
               i != e; ++i) {
      strm << (*i)->GetName()
          << static_cast<const char*>("(")
          << (*i)->getType().getName()
          << static_cast<const char*>("),");
   }

   return strm;
}
} //namespace kuafu
