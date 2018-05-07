#ifndef KUAFU_MACHINE_H_
#define KUAFU_MACHINE_H_

#include <vector>
#include <type_traits>

#include "fsm/machine_type.h"
#include "fsm/fsmtype.h"
#include "log/externallogger.h"

namespace kuafu {

template<typename M, typename... Args>
std::shared_ptr<typename std::decay<M>::type> MakeMachine(Args&&... args) {
    using RealType = typename std::decay<M>::type;
    std::shared_ptr<RealType> machine = std::make_shared<RealType>(std::forward<Args>(args)...);
    if (machine) {
        machine->Birth();
    }

    return machine;
}

class MachineBase : public ExternalLogger {
 public:
   MachineBase(const std::string& name)
      :type_(name)
      ,name_(name) {
   }

   virtual ~MachineBase() {}

 public:
   virtual bool Process(EventSharedPtr) = 0;
   virtual void SetTimeout(unsigned long long timeout_ms) = 0;
   virtual void Birth() = 0; 

   virtual unsigned long long GetTimeout() const {
       return 0;
   }

   virtual bool IsTimeout() const {
       return false;
   }

 public:
   const MachineType& GetType() const {
       return type_;
   }

   const std::string& GetName() const {
       return name_; 
   }

   void SetName(const std::string& name) { 
       name_ = name;
   }

 protected:
   MachineType type_;
   std::string name_;
};


class StateMachine : public MachineBase {
 public:
   friend class State;
 public:
   StateMachine(const std::string& name);

 public:
   virtual ~StateMachine(){}

 public:
   bool ForceState(const StateSharedPtr& state);
   bool SetStartState(const StateSharedPtr& state);
   bool SetMetaState(const StateSharedPtr& state);
   void ClearMetaState();
   const StateSharedPtr GetCurrent() const;
   const StateSharedPtr GetPrevious() const {
       return previous_state_;
   }

 public:
   virtual void SetTimeout(unsigned long long timeout_ms);
   virtual unsigned long long GetTimeout() const;
   virtual bool IsTimeout() const;
   virtual bool Process(EventSharedPtr);

 protected:
   StateMachine(const StateMachine& rhs);

 private:
   StateMachine& operator=(const StateMachine& rhs);

 private:
   bool ProcessNormalStateTransition(EventSharedPtr event);
   bool ProcessMetaStateTransition(EventSharedPtr event);

   template<bool IsMetaState>
   bool InternalSetState(const StateSharedPtr& state);

 protected:
   unsigned long long timeout_ms_;
   StateSharedPtr current_state_;
   StateSharedPtr previous_state_;
   
   typedef std::vector<StateSharedPtr> StateListType;
   StateListType states_;

   StateSharedPtr meta_state_;
};

class ActionMachine : public MachineBase {
   friend class NonTransitiveAction;

 public:
   ActionMachine(const std::string& name)
      :MachineBase(name) {
   }

 public:
   virtual bool Process(EventSharedPtr);

 protected:
   typedef std::vector<NonTransitiveActionSharedPtr> ActionListType;
   ActionListType non_transitive_actions_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_MACHINE_H_
