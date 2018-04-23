#ifndef KUAFU_TRANSITION_H_
#define KUAFU_TRANSITION_H_

#include <functional>
#include <string>

#include "util/noncopyable.h"
#include "fsm/fsmtype.h"

namespace kuafu {

class MachineType;
//-------------------------------------------------------------------
typedef std::function<void(MachineBase&,
            State&,
            ITransition&,
            Event*
            Statu&)> TransitionFireType;

typedef std::funciton<void(MachineBase&, ITransition&, Event*)> ActionFireType;
//-------------------------------------------------------------------
class ITransition : public NonCopyableForAll {
 public:
     virtual ~ITransition() {}

     virtual bool IsMatch(const EventSharedPtr& event,
                 const MachineBase& machine) = 0;
     virtual bool IsValid() const = 0;
     virtual const std::string& GetName() const = 0;
};
//-------------------------------------------------------------------
TransitionSharedPtr MakeTransition() (const char* name,
                const StateSharedPtr& from,
                const StateSharedPtr& to,
                IPredicateSharedPtr&& pred);

TransitionSharedPtr MakeTransition(const char* name,
            const StateShared& toFrom,
                IPredicateSharedPtr&& pred);
//-------------------------------------------------------------------
class Transition : public ITransition,
    public std::enable_shared_from_this<Transition> {
    friend class StateMachine;

 public:
    enum class TransitionType {
        TT_NORMAL_TRANSITION,
        TT_INTERNAL_TRANSITION,
    };

 public:
    virtual ~Transition();

 private:
    Transition(const char* name,
                const StateSharedPtr& from,
                const StateSharedPtr& to,
                IPredicateSharedPtr&& pred);

    Transition(const char* name,
                const StateSharedPtr& to_from,
                IPredicateSharedPtr&& pred);

 public:
    virtual bool IsValid() const { 
        return pred_ && is_valid_;
    }

    virtual bool IsMatch(const Event* event, const MachineBase& machine);
    virtual const std::string& GetName() const {
        return name_;
    }

 public:

    TransitionType GetTransitionType() const {
        return transition_type_;
    }

    void ClearActions();

 public:
    TransitionFireType OnTransition;

 private:
    void Init();

 private:
    TransitionType transition_type_;
    std::string name_;
    IPredicateSharedPtr pred_;

    StateWeakPtr to_;
    StateWeakPtr hold_from_;
    StateWeakPtr hold_to_;

    bool is_valid_;
};
//-------------------------------------------------------------------
NonTransitiveActionSharedPtr MakeNonTransitiveAction(const char* name,
            ActionMachine& ownerMachine, 
            IPredicateSharedPtr&& pred);

class NonTransitiveAction : public ITransition,
    public std::enable_shared_from_this<Transition> {
   friend class ActionMachine;

 public:
   virtual ~NonTransitiveAction();

 private:
   NonTransitiveAction(const char* name,
               ActionMachine& ownerMachine, 
               IPredicateSharedPtr&& pred);

 public:
   virtual bool IsValid() const {
       return pred_;
   }

   virtual bool IsMatch(const Event* event, const MachineBase& machine);

   virtual const std::string& GetName() const { return name_; }

   void ClearActions();

 public:
   ActionEvent OnAction;

 private:
   void init(ActionMachine& ownerMachine);

 private:
   std::string name_;
   IPredicateSharedPtr pred_;

 private:
   NonTransitiveAction& operator=(const NonTransitiveAction&);
};

} // namespace kuafu

#endif // #ifndef KUAFU_TRANSITION_H_
