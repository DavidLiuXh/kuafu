#ifndef KUAFU_MACHINE_H_
#define KUAFU_MACHINE_H_

#include <vector>

#include "fsm/machine_type.h"
#include "log/externallogger.h"

namespace kuafu {

class MachineBase : public ExternalLogger {
 public:
   MachineBase(const MachineType& machine_type,
      const std::string& name)
      :type_(machine_type)
      ,name_(name) {
   }

   virtual ~MachineBase() {}

 public:
   virtual bool Process(EventSharedPtr) = 0;
   virtual void SetTimeout(unsigned long long timeout_ms) = 0;

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

   const string& GetName() const {
       return name_; 
   }

   void SetName(const std::string& name) { 
       name_ = name;
   }

 protected:
   const MachineType type_;
   std::string name_;
};


class StateMachine : public MachineBase {
   friend class State;

 public:
   StateMachine(const MachineType& type,
               const std::string& name);
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
   bool ProcessNormalStateTransition(Event* event);
   bool ProcessMetaStateTransition(Event* event);

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
   ActionMachine(const MachineType& type,
      const std::string& name)
      :MachineBase(type, name) {
   }

 public:
   virtual bool process(Event*);

 protected:
   typedef vector<NonTransitiveActionSharedPtr> ActionListType;
   ActionListType non_transitive_actions_;
};

} // namespace kuafu

#endif // #ifndef KUAFU_MACHINE_H_
