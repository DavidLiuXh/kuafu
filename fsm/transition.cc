#include "fsm/transition.h"

#include <algorithm>
#include <memory>

#include "util/strutil.h"
#include "fsm/machine.h"
#include "fsm/state.h"
#include "fsm/transition_predicate.h"

namespace kuafu {

TransitionSharedPtr Transition::MakeTransition(const char* name,
                const StateSharedPtr& from,
                const StateSharedPtr& to,
                IPredicateSharedPtr pred) {
    TransitionSharedPtr transition(new Transition(
                name,
                from,
                to,
                pred));
    if (transition) {
        transition->Init();
    }

    return transition;
}

TransitionSharedPtr Transition::MakeTransition(const char* name,
            const StateSharedPtr& from,
                IPredicateSharedPtr pred) {
    TransitionSharedPtr transition(new Transition(
                name,
                from,
                pred));
    if (transition) {
        transition->Init();
    }

    return transition;
}
//-------------------------------------------------------------------
Transition::Transition(const char* name,
            const StateSharedPtr& from,
            const StateSharedPtr& to,
            IPredicateSharedPtr pred)
:transition_type_(TransitionType::TT_NORMAL_TRANSITION)
,name_(StrUtil::SafeGetString(name))
,pred_(pred)
,to_(to)
,hold_from_(from)
,hold_to_(to)
,is_valid_(true) {
}

Transition::Transition(const char* name,
            const StateSharedPtr& to_from,
                       IPredicateSharedPtr pred)
:transition_type_(TransitionType::TT_INTERNAL_TRANSITION)
,name_(StrUtil::SafeGetString(name))
,pred_(pred)
,hold_from_(to_from)
,hold_to_(to_from)
,is_valid_(true) {
}

Transition::~Transition() {
}

void Transition::Init() {
    StateSharedPtr hold_from_state = hold_from_.lock(); 
    if (hold_from_state) {
        auto transitions = hold_from_state->transitions_;
        if (std::find(transitions.begin(),
                            transitions.end(),
                            shared_from_this()) == transitions.end()) {
            transitions.push_back(shared_from_this());
            to_ = hold_to_;
        }
    }
}

bool Transition::IsMatch(const EventSharedPtr& event,
                    const MachineBase& machine) {
    bool rt = false;
    if (pred_) {
        rt = (*pred_)(event, machine);
    }

    return rt;
}

void Transition::ClearActions() {
    OnTransition = nullptr;
}
//-------------------------------------------------------------------
NonTransitiveActionSharedPtr NonTransitiveAction::MakeNonTransitiveAction(const char* name,
            ActionMachine& owner_machine, 
            IPredicateSharedPtr pred) {
    NonTransitiveActionSharedPtr non_transition(new NonTransitiveAction(
                name,
                pred));
    if (non_transition) {
        non_transition->Init(owner_machine);
    }

    return non_transition;
}

NonTransitiveAction::NonTransitiveAction(const char* name,
            IPredicateSharedPtr pred)
:name_(StrUtil::SafeGetString(name))
,pred_(pred) {
}

NonTransitiveAction::~NonTransitiveAction() {
}

void NonTransitiveAction::Init(ActionMachine& owner_machine) {
   auto found = std::find(owner_machine.non_transitive_actions_.begin(),
               owner_machine.non_transitive_actions_.end(),
               shared_from_this());
   if (found == owner_machine.non_transitive_actions_.end()) {
      owner_machine.non_transitive_actions_.push_back(shared_from_this());
   }
}

bool NonTransitiveAction::IsMatch(const EventSharedPtr& event,
                             const MachineBase& machine) {
    bool rt = false;

    if (pred_) {
      rt = (*pred_)(event, machine);
    }

    return rt;
}

void NonTransitiveAction::ClearActions() {
   OnAction = nullptr;
}

} //namespace kuafu
