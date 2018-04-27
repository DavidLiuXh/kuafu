#include "fsm/state.h"
#include "fsm/machine.h"

#include "util/strutil.h"

namespace kuafu {

StateSharedPtr State::MakeState(StateMachine& owner, const char* name, time_t time_ms) {
    //StateSharedPtr state = std::make_shared<State>(owner, name, time_ms);
    StateSharedPtr state(new State(owner,
                    name,
                    time_ms));
    if (state) {
        owner.states_.push_back(state);
    }

    return state;
}

StateSharedPtr State::MakeState(StateMachine& owner, const State& copy) {
    //StateSharedPtr state = std::make_shared<State>(owner, copy);
    StateSharedPtr state(new State(owner, copy));
    if (state) {
        owner.states_.push_back(state);
    }

    return state;
}

State::State(StateMachine& owner, const char* name, time_t timeout_ms)
:name_(StrUtil::SafeGetString(name))
,timeout_ms_(timeout_ms) {
}

State::State(StateMachine& owner, const State& copy)
:name_(copy.name_)
,timeout_ms_(copy.timeout_ms_) {
}

State::~State() {
}

void State::ClearActions() {
   OnEnter = nullptr;
   OnExit = nullptr;
}

bool State::operator==(const State& rhs) const {
   return this == &rhs;
}

bool State::operator!=(const State& rhs) const {
   return this != &rhs;
}

} //namespace kuafu
