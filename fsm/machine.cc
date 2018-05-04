#include "fsm/machine.h"

#include <algorithm>

#include "fsm/event.h"
#include "fsm/state.h"
#include "fsm/machine_set.h"
#include "fsm/transition.h"
#include "util/timeutil.h"

namespace kuafu {

StateMachine::StateMachine(const std::string& name)
                           :MachineBase(name),
                           timeout_ms_(0) {
}

StateMachine::StateMachine(const StateMachine& rhs)
:MachineBase(rhs),
timeout_ms_(rhs.timeout_ms_) {
}

bool StateMachine::ForceState(const StateSharedPtr& state) {
    return InternalSetState<false>(state);
}

bool StateMachine::SetStartState(const StateSharedPtr& state) {
    return InternalSetState<false>(state);
}

template<bool IsMetaState>
bool StateMachine::InternalSetState(const StateSharedPtr& state) {
    bool rt = false;

    auto found = std::find(states_.begin(),
                states_.end(),
                state);
    if (states_.end() != found) {
        if (IsMetaState) {
            meta_state_ = state;;
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
      timeout_ms_ = timeout_ms != 0 ? TimeUtil::GetTimeMs() + timeout_ms : 0;
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

bool StateMachine::Process(EventSharedPtr event) {
    ExternalDebugLog("Process StateMachine name: " << GetName()
                << "| Machine type:"
                <<  GetType().GetName() << "| Current State: " << (current_state_ ? current_state_->GetName() : "nil"));

   if (ProcessNormalStateTransition(event)) {
      return true;
   }

   return ProcessMetaStateTransition(event);
}

bool StateMachine::ProcessNormalStateTransition(EventSharedPtr event) {
    if (current_state_) {
        TransitionSharedPtr transition = nullptr;

        for (auto i = current_state_->transitions_.begin();
                    i != current_state_->transitions_.end(); ++i) {
            transition = *i;
            StateSharedPtr to_status = transition->to_.lock();

            if (transition->IsMatch(event, *this)) {
                ExternalInfoLog("Event (match) type: " << event->ToString()
                            << ", Type: " << type_.GetName()
                            << ", Name: " << name_);

                if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) {
                    std::stringstream ss;
                    ss << "at Exit action: (" << type_.GetName()
                        << " " << name_ << ") "
                        << current_state_->GetName();

                    ExternalDebugLog("Entering " << ss.str());
                    try {
                        current_state_->OnExit(*this, current_state_);
                        ExternalDebugLog("Exited " << ss.str());
                    } catch (...) {
                        ExternalErrorLog("Caught exception " << ss.str());
                    }

                    ss.str("");

                    ExternalInfoLog("Normal Transition from: " << current_state_->GetName()
                                << " -> " << (to_status ? to_status->GetName() : "non_status")
                                << ", for Machine ("
                                << type_.GetName() << " " << name_ << ") ");
                    ExternalDebugLog("Transition: ["
                                << transition->GetName()
                                << "]");
                } else {
                    ExternalInfoLog("Internal Transition for state: " << current_state_->GetName()
                                << ", for Machine"
                                << " (" << type_.GetName() << " " << name_ << ") ");
                    ExternalDebugLog("Transition: ["
                                << transition->GetName()
                                << "]");
                }

                try {
                    transition->OnTransition(*this,
                                current_state_,
                                std::static_pointer_cast<ITransition>(transition),
                                event,
                                to_status);
                    if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) {
                        ExternalDebugLog("Exited Normal Transition: ["
                                    << transition->GetName()
                                    << "] from: " << current_state_->GetName()
                                    << " -> " << (to_status ? to_status->GetName() : "non_status")
                                    << ", for Machine"
                                    << " (" << type_.GetName() << " " << name_ << ") ");
                    } else {
                        ExternalDebugLog("Exited Internal Transition: ["
                                    << transition->GetName()
                                    << "] for state: " << current_state_->GetName()
                                    << ", for Machine"
                                    << " (" << type_.GetName() << " " << name_ << ") ");
                    }
                } catch (...) {
                    if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) {
                        ExternalErrorLog("Caught exception at Normal Transition action: ["
                                    << transition->GetName()
                                    << "] from: " << current_state_->GetName()
                                    << " -> " << (to_status ? to_status->GetName() : "non_status")
                                    << ", for Machine"
                                    << " (" << type_.GetName() << " " << name_ << ") ");
                    } else {
                        ExternalErrorLog("Caught exception at Internal Transition action: ["
                                    << transition->GetName()
                                    << "] for state: " << current_state_->GetName()
                                    << ", for Machine"
                                    << " (" << type_.GetName() << " " << name_ << ") ");
                    }
                }

                previous_state_ = current_state_;
                current_state_ = transition->to_.lock();

                if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) { {
                    MachineSetSharedPtr machine_set = event->machine_set_.lock();
                    if (machine_set) {
                        machine_set->UpdateTimeoutMahcine(*this, current_state_->GetTimeout());
                        SetTimeout(current_state_->GetTimeout());
                    }

                    ExternalDebugLog("Entering Enter action: (" << type_.GetName()
                                << " " << name_
                                << ") " << (to_status ? to_status->GetName() : "non_status"));

                    try {
                        current_state_->OnEnter(*this, current_state_);
                        ExternalDebugLog("Exited Enter action: (" << type_.GetName()
                                    << " " << name_ << ") "
                                    << (to_status ? to_status->GetName() : "non_status"));
                    } catch (...) {
                        ExternalErrorLog("Caught exception at Enter action: (" << type_.GetName() << " "
                                    << name_ << ") " << current_state_->GetName());
                    }
                }

                return true;
                }
            } else {
                ExternalDebugLog("no match");
            }
        }
    }

   return false;
}

bool StateMachine::ProcessMetaStateTransition(EventSharedPtr event) {
   // check meta transitions AFTER specific transitions
   if (meta_state_ && current_state_) {
      for (auto i = meta_state_->transitions_.begin();
         i != meta_state_->transitions_.end(); ++i) {
         TransitionSharedPtr transition = *i;
         StateSharedPtr to_status = transition->to_.lock();

         if (transition->IsMatch(event, *this)) {
            if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) {
                std::stringstream ss;
                ss << "at Exit action: (" << type_.GetName()
                    << " " << name_ << ") "
                    << current_state_->GetName();

               ExternalDebugLog("Entering Meta " << ss.str());
               try {
                  current_state_->OnExit(*this, current_state_);
                  ExternalDebugLog("Exited Meta " << ss.str());
               } catch (...) {
                  ExternalErrorLog("Caught exception " << ss.str());
               }
            }

            ExternalInfoLog( "Meta Transition from: Empty -> " << (to_status ? to_status->GetName() : "non-status")
               << ", for Machine"
               << " (" << type_.GetName() << " " << name_ << ") ");
            ExternalDebugLog( "Meta Transition: [" << transition->GetName()
               << "]");

            try {
               transition->OnTransition(
                  *this,
                  current_state_,
                  transition,
                  event,
                  to_status);
               ExternalDebugLog( "Exited Meta Transition: [" << transition->GetName()
                  << "] from: Empty -> " << (to_status ? to_status->GetName() : "non-status")
                  << ", for Machine"
                  << " (" << type_.GetName() << " " << name_ << ") ");
            } catch (...) {
               ExternalErrorLog( "Caught exception at Meta Transition action: [" << transition->GetName()
                  << "] from: Empty -> " << (to_status ? to_status->GetName() : "non-status")
                  << ", for Machine"
                  << " (" << type_.GetName() << " " << name_ << ") ");
            }

            current_state_ = to_status;

            // transitions to meta-state do not change the current state
            if (transition->GetTransitionType() == Transition::TransitionType::TT_NORMAL_TRANSITION) {
                MachineSetSharedPtr machine_set = event->machine_set_.lock();
               // reset timer only on entry
               if (machine_set &&
                           to_status) {
                  machine_set->UpdateTimeoutMahcine(*this, to_status->GetTimeout());
                  SetTimeout(to_status->GetTimeout());
               }

               ExternalDebugLog("Entering Meta Enter action: (" << type_.GetName() << " "
                           << name_ << ") "
                           << current_state_->GetName());
               try {
                  current_state_->OnEnter(*this, current_state_);
                  ExternalDebugLog("Exited Meta Enter action: (" << type_.GetName() << " "
                              << name_ << ") "
                              << current_state_->GetName());
               } catch (...) {
                  ExternalErrorLog("Caught exception at Meta Enter action: (" << type_.GetName() << " "
                              << name_ << ") "
                              << current_state_->GetName());
               }
            }
            return true;
         }
      }
   }

   return false;
}

bool ActionMachine::Process(EventSharedPtr event) {
   ExternalDebugLog("Process ActionMachine name: " << GetName().c_str() << "| Machine type:"
      <<  GetType().GetName().c_str());

   for(auto i = non_transitive_actions_.begin();
      i != non_transitive_actions_.end(); ++i) {
      NonTransitiveActionSharedPtr non_transitive_transition = *i;

      if (non_transitive_transition->IsMatch(event, *this)) {
         ExternalInfoLog("Non-transitive action: " << non_transitive_transition->GetName()
            << " for "
            << " (" << type_.GetName() << " " << name_ << ") ");

         try {
             non_transitive_transition->OnAction(*this,
                         std::static_pointer_cast<ITransition>(non_transitive_transition),
                         event);
             ExternalDebugLog("Exited Non-transitive action: " << non_transitive_transition->GetName()
                         << " for "
                         << " (" << type_.GetName() << " " << name_ << ") ");
         } catch (...) {
            ExternalErrorLog("Caught exception at Non-transitive action: " << non_transitive_transition->GetName()
               << " for (" << type_.GetName() << " " << name_ << ") ");
         }

         return true;
      }
   }

   return false;
}

}//namespace kuafu
