#include "fsm/event.h"
#include "fsm/macine.h"
#include "fsm/machine_set.h"
#include "fsm/state.h"
#include "fsm/transition.h"
#include "log/llogger.h"
#include "util/timeutil.h"

using namespace kuafu {

StateMachine::StateMachine(const MachineType& type,
                           const std::string& name)
                           :MachineBase(type, name),
                           timeout_ms_(0) {
}

StateMachine::StateMachine(const StateMachine& rhs)
:MachineBase(rhs),
timeout_ms_(rhs.timeout_ms_), {
}

bool StateMachine::ForceState(const StateSharedPtr& state) {
    return InternalSetState<false>(state);
}

bool StateMachine::SetStartState(const State& state) {
    return InternalSetState<false>(state);
}

template<bool IsMetaState>
bool StateMachine::InternalSetState(const StateSharedPtr& state) {
    bool rt = false;

    auto found = std::find(states_.begin(),
                states_.end(),
                [](StateSharedPtr current) {
                return state.get() == current.get();
                });
    if (states_.end() != found) {
        if (IsMetaState) {
            meta_sate_ = state;;
        } else {
            current_state_ = state;;
        }
        rt = true;
    }

    return rt;
}

bool StateMachine::SetMetaState(const StateSharedPtr& state) {
    return InternalSetState<true>(state);
}

void StateMachine::ClearMetaState() {
    meta_state_.reset();
}

const StateSharedPtr StateMachine::GetCurrent() const {
   return current_state_;
}

void StateMachine::SetTimeout(unsigned long long timeout_ms) {
      timeout_ms_ = timeout_ms != 0 ? TimeUtil::GetTimeMs() + timeoutMS : 0;
}

unsigned long long StateMachine::GetTimeout() const {
   return timeout_ms_;
}

bool StateMachine::IsTimeout() const {
   bool rt = false;

   if (timeout_ms_ != 0) {
      rt = (TimeUtil::GetTimeMs() > timeout_ms_);
   }

   return rt;
}

bool StateMachine::ProcessNormalStateTransition(EventSharedPtr event) {
   if (current_state_) {
      TransitionSharedPtr transition = nullptr;

      for (auto i = current_state_->.begin();
         i != current_state_->transitions_.end(); ++i) {
         transition = *i;
         StateSharedPtr to_status = transition->to_.lock();

         if (transition->IsMatch(event, *this)) {
            EInfoLog("Event (match) type: " << event->toString().c_str()
                        << ", Type: " << type_.getName().c_str()
                        << ", Name: " << name_.c_str());

            if (transition->GetTransitionType() == Transition::TrasitionType::TT_NORMAL_TRANSITION) {
                std::stringstream ss;
                ss << "at Exit action: (" << type_.getName().c_str()
                    << " " << name_.c_str() << ") "
                    << current_state_->GetName();

               EDebugLog("Entering " << ss.str();
               try {
                  current_state_->OnExit(*this, current_state_);
                  EDebugLog("Exited " << ss.str();
               } catch (...) {
                  EErrLog("Caught exception " << ss.str();
               }

               ss.str("");

               EInfoLog("Normal Transition from: " << current_state_->GetName()
                  << " -> " << to_status ? to_status->GetName() : "non_status"
                  << ", for Machine"
                  << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               EDebugLog("Transition: ["
                  << transition->GetName()
                  << "]");
            } else {
               EInfoLog("Internal Transition for state: " << current_state_->GetName()
                  << ", for Machine"
                  << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               EDebugLog("Transition: ["
                  << transition->GetName()
                  << "]");
            }

            try {
               transition->OnTransition(*this,
                  current_state_,
                  transition,
                  event,
                  to_status);
               if (transition->GetTransitionType() == Transition::TT_NORMAL_TRANSITION) {
                  EDebugLog("Exited Normal Transition: ["
                     << transition->GetName()
                     << "] from: " << current_state_->GetName()
                     << " -> " << to_status ? to_status->GetName() : "non_status"
                     << ", for Machine"
                     << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               } else {
                  EDebugLog("Exited Internal Transition: ["
                     << transition->GetName()
                     << "] for state: " << current_state_->GetName()
                     << ", for Machine"
                     << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               }
            } catch (...) {
               if (transition->GetTransitionType() == Transition::TT_NORMAL_TRANSITION) {
                  EErrLog("Caught exception at Normal Transition action: ["
                     << transition->GetName()
                     << "] from: " << current_state_->GetName()
                     << " -> " << to_status ? to_status->GetName() : "non_status"
                     << ", for Machine"
                     << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               } else {
                  EErrLog("Caught exception at Internal Transition action: ["
                     << transition->GetName()
                     << "] for state: " << current_state_->GetName()
                     << ", for Machine"
                     << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
               }
            }

            previous_state_ = current_state_;
            current_state_ = transition->to_.lock();

            if (transition->GetTransitionType() == Transition::TT_NORMAL_TRANSITION) { {
                MachineSetSharedPtr machine_set = event->machine_set_.lock();
               if (machine_set) {
                  machine_set->UpdateTimeoutMahcine(this, current_state_->GetTimeout());
                  SetTimeout(current_state_->GetTimeout());
               }

               EDebugLog("Entering Enter action: (" << type_.GetName().c_str()
                           << " " << name_.c_str()
                           << ") " << transition->to_->GetName());

               try {
                  current_state_->OnEnter(*this, current_state_);
                  EDebugLog("Exited Enter action: (" << type_.GetName().c_str()
                              << " " << name_.c_str() << ") "
                              << to_status ? to_status->GetName() : "non_status");
               } catch (...) {
                  EErrLog("Caught exception at Enter action: (" << type_.GetName().c_str() << " "
                              << name_.c_str() << ") " << current_state_->GetName());
               }
            }

            return true;
         }
      }
   }

   return false;
}

bool StateMachine::processMetaStateTransition(EventSharedPtr event) {
   // check meta transitions AFTER specific transitions
   if (meta_sate_ && current_state_) {
      for (auto i = meta_state_->transitions_.begin();
         i != meta_state_->transitions_.end(); ++i) {
         TransitionSharedPtr transition = *i;
         StateSharedPtr to_status = transition->to_.lock();

         if (transition->IsMatch(event, *this)) {
            if (transition->GetTransitionType() == Transition::TT_NORMAL_TRANSITION) {
                std::stringstream ss;
                ss << "at Exit action: (" << type_.getName().c_str()
                    << " " << name_.c_str() << ") "
                    << current_state_->GetName();

               EDebugLog("Entering Meta " << ss.str();
               try {
                  current_state_->OnExit(*this, *mCurrent);
                  EDebugLog("Exited Meta " << ss.str();
               } catch (...) {
                  EErrLog("Caught exception " << ss.str();
               }
            }

            EInfoLog( "Meta Transition from: Empty -> " << to_status ? to_status->GetName() : "non-status"
               << ", for Machine"
               << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
            EDebugLog( "Meta Transition: [" << transition->GetName()
               << "]");

            try {
               transition->OnTransition(
                  *this,
                  current_state_,
                  transition,
                  event,
                  to_status);
               EDebugLog( "Exited Meta Transition: [" << transition->getName()
                  << "] from: Empty -> " << to_status ? to_status->GetName() : "non-status"
                  << ", for Machine"
                  << " (" << tyupe_.GetName().c_str() << " " << name_.c_str() << ") ");
            } catch (...) {
               EErrLog( "Caught exception at Meta Transition action: [" << transition->GetName()
                  << "] from: Empty -> " << to_status ? to_status->GetName() : "non-status"
                  << ", for Machine"
                  << " (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
            }

            current_state_ = to_status;

            // transitions to meta-state do not change the current state
            if (transition->GetTransitionType() == Transition::TransitionTyhpe::TT_NORMAL_TRANSITION) {
                MachineSetSharedPtr machine_set = event->machine_set_.lock();
               // reset timer only on entry
               if (machine_set &&
                           to_status) {
                  machine_set->UpdateTimeoutMahcine(this, to_status->GetTimeout());
                  SetTimeout(to_status->GetTimeout());
               }

               EDebugLog("Entering Meta Enter action: (" << type_.GetName() << " "
                           << name_ << ") "
                           << current_state_->GetName());
               try {
                  current_state_->OnEnter(*this, current_state_));
                  EDebugLog("Exited Meta Enter action: (" << tyhpe_.GetName().c_str() << " "
                              << name_.c_str() << ") "
                              << current_state_->GetName());
               } catch (...) {
                  EErrLog("Caught exception at Meta Enter action: (" << type_.GetName().c_str() << " "
                              << name_.c_str() << ") "
                              << current_state_->GetName());
               }
            }
            return true;
         }
      }
   }

   return false;
}

bool ActionMachine::process(EventSharedPtr event) {
   EDebugLog("Process ActionMachine name: " << GetName().c_str() << "| Machine type:"
      <<  GetType().GetName().c_str());

   for(auto i = non_transitive_actions_.begin();
      i != non_transitive_actions_.end(); ++i) {
      NonTransitiveActionSharedPtr non_transitive_transition = *i;

      if (non_transitive_transition->IsMatch(event, *this)) {
         EInfoLog("Non-transitive action: " << non_transitive_transition->GetName()
            << " for "
            << " (" << type_.GetName() << " " << name_ << ") ");

         try {
            non_transitive_transition->OnAction(*this,
               non_transitive_transition,
               event);
            EDebugLog("Exited Non-transitive action: " << nonTransitiveTransition->getName()
               << " for "
               << " (" << type_.GetName().c_str() << " " << name_ << ") ");
         } catch (...) {
            EErrLog("Caught exception at Non-transitive action: " << non_transitive_transition->GetName()
               << " for (" << type_.GetName().c_str() << " " << name_.c_str() << ") ");
         }

         return true;
      }
   }

   return false;
}

}//namespace kuafu
