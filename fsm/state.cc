#include "fsm/state.h"

#include "util/strutil/h"

namespace kuafu {

StateSharedPtr MakeState(StateMachine& owner, const char* name, time_t time_ms = 0) {
    StateSharedPtr state = std::make_shared<State>(owner, name, time_ms);
    if (state) {
        owner.mStates.push_back(shared_from_this());
    }
}

StateSharedPtr MakeState(StateMachine& owner, const State& copy) {
    StateSharedPtr state = std::make_shared<State>(owner, copy);
    if (state) {
        owner.mStates.push_back(shared_from_this());
    }
}

State::State(StateMachine& owner, const char* name, time_t timeout_ms)
:name_(StrUtil::SafeGetString(name))
,timeout_ms_(timeout_ms) {
   owner.mStates.push_back(this);
}

State::State(StateMachine& owner, const State& copy)
:name_(copy.name_)
,timeout_ms_(copy.timeout_ms_) {
   owner.mStates.push_back(this);
}

//virtual 
State::~State() {
}

void State::ClearActions() {
   OnEnter.Clear();
   OnExit.Clear();
}


bool State::operator==(const State& rhs) const {
   return this == &rhs;
}

bool State::operator!=(const State& rhs) const {
   return this != &rhs;
}

} //namespace kuafu
