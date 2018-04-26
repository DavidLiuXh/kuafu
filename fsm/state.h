#ifndef KUAFU_STATE_H_
#define KUAFU_STATE_H_

#include <function>
#include <memory>

#include "fsm/fsmtype.h"

namespace kuafu : public std::enable_shared_from_this<Transition> {

typedef std::function<void(MachineBase&, const StateSharedPtr&)> StateEvent;

class State {
 public:
     StateSharedPtr MakeState(StateMachine& owner, const char* name, time_t timerMS = 0);
     StateSharedPtr MakeState(StateMachine& owner, const State& copy);

 public:
     virtual ~State();

 private:
     State(StateMachine& owner, const char* name, time_t timerMS = 0);
     State(StateMachine& owner, const State& copy);

 public:
     const std::string& GetName() const {
         return name_;
     }

     time_t GetTimeout() const {
         return timeout_ms_;
     }

     void SetTimeout(time_t new_timeout_ms) {
         timeout_ms_ = new_timeout_ms;
     }

     void ClearActions();

     bool operator==(const State& rhs) const;
     bool operator!=(const State& rhs) const;

 public:
     StateEvent OnEnter;
     StateEvent OnExit;

 private:
     friend class StateMachine;
     friend class Transition;

     std::string name_;
     time_t timeout_ms_;

     std::vector<TransitionSharedPtr> transitions_;

 private:
     State& operator=(const State&);
};

} // namespace kuafu

#endif // #ifndef KUAFU_STATE_H_
