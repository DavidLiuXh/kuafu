#include "fsm/transition.h"

#include "util/strutil.h"

#include "fsm/machine.h"
#include "fsm/state.h"
#include "fsm/transition_predicate.h"

namespace kuafu {

TransitionSharedPtr MakeTransition() (const char* name,
                const StateSharedPtr& from,
                const StateSharedPtr& to,
                IPredicateSharedPtr&& pred) {
    TransitionSharedPtr transition = std::make_shared<Transition>(
                name,
                from,
                to,
                pred);
    if (transition) {
        transition->Init();
    }
}

TransitionSharedPtr MakeTransition(const char* name,
            const StateShared& toFrom,
                IPredicateSharedPtr&& pred) {
    TransitionSharedPtr transition = std::make_shared<Transition>(
                name,
                from,
                pred);
    if (transition) {
        transition->Init();
    }
}
//-------------------------------------------------------------------
Transition::Transition(const char* name,
            const StateSharedPtr& from,
            const StateSharedPtr& to,
            IPredicateSharedPtr&& pred)
:transition_type_(TransitionType::TT_NORMAL_TRANSITION)
,name_(StrUtil::SafeGetString(name))
,pred_(pred)
,to_(to)
,hold_from_(from)
,hold_to_(to)
,is_valid_(true) {
}

Transition::Transition(const char* name,const StateSharedPtr& to_from,
                       IPredicateSharedPtr&& pred)
:transition_type_(TT_INTERNAL_TRANSION)
,name_(StrUtil::SafeGetString(name))
,pred_(pred)
,hold_from_(&to_from)
,hold_to_(&to_from)
,is_valid_(true) {
}

Transition::~Transition() {
}

void Transition::Init() {
    StateSharedPtr hold_from_state = hold_from_.lock(); 
    if (hold_from_state) {
        TransitionSharedPtr transitions = hold_from_state->transitions_;
        if (Transitions &&
                    std::find(hold_from_state->begin(),
                        hold_from_state->end(),
                        [this](TransitionSharedPtr& t) {
                        return t.get() == this;
                        })) {
            hold_from_state->transitions_.push_back(shared_from_this());
            to_ = hold_to_;
        }
    }
}

bool Transition::IsMatch(const Event* event,
                    const MachineBase& machine) {
    bool rt = false;
    if (mPred) {
        rt = (*mPred)(event, machine);
    }

    return rt;
}

void Transition::ClearActions() {
    OnTransition = nullptr;
}

//-------------------------------------------------------------------
NonTransitiveActionSharedPtr MakeNonTransitiveAction(const char* name,
            ActionMachine& ownerMachine, 
            IPredicateSharedPtr&& pred) {
    NonTransitiveActionSharedPtr non_transition = std::make_shared<NonTransitiveAction>(
                name,
                ownerMachine,
                pred);
    if (non_transition) {
        non_transition->Init();
    }
}

NonTransitiveAction::NonTransitiveAction(const char* name,
            ActionMachine& ownerMachine,
            IPredicateSharedPtr&& pred)
:name_(StrUtil::SafeGetString(name))
,pred_(pred) {
}

NonTransitiveAction::~NonTransitiveAction() {
}

void NonTransitiveAction::Init(ActionMachine& ownerMachine) {
   auto found = std::find(ownerMachine.mNonTransitiveActions.begin(),
               ownerMachine.mNonTransitiveActions.end(),
               [this](NonTransitionSharedPtr& non_transition) {
               return non_transition.get() == this;
               });
   if (found == ownerMachine.mNonTransitiveActions.end()) {
      ownerMachine.mNonTransitiveActions.push_back(shared_from_this());
   }
}

bool NonTransitiveAction::IsMatch(const Event* event,
                             const MachineBase& machine) {
    bool rt = false;

    if (mPred)
      rt = (*mPred)(event, machine);

    return rt;
}

void NonTransitiveAction::ClearActions() {
   OnAction = nullptr;
}

} //namespace kuafu
