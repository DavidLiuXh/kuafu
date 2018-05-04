#include <string>
#include <sstream>

#include "log/kflogger.h"

#include "fsm/fsmtype.h"
#include "fsm/event.h"
#include "fsm/add_machine_event.h"
#include "fsm/state.h"
#include "fsm/transition.h"
#include "fsm/transition_predicate.h"
#include "fsm/timeout_predicate.h"
#include "fsm/machine.h"
#include "fsm/machine_set.h"

//----------------------------------------------------------
enum class FoodEventType {
    FET_LOGIN,
    FET_LOGIN_FAILED,
    FET_LOGIN_OK,
    FET_LOGOUT,
};

class FoodEvent : public kuafu::EventTemplate<FoodEventType> {
    public:
        using underlying_type = std::underlying_type<FoodEventType>::type;

        FoodEvent(FoodEventType type, const kuafu::MachineBaseSharedPtr& machine)
            :kuafu::EventTemplate<FoodEventType>(type, machine) {
            }

        virtual std::ostream& ToStream(std::ostream& str) const {
            return str << "FoodEvent | " << static_cast<underlying_type>(type_) << std::endl;
        }

        virtual std::string ToString() const {
            std::ostringstream os;
            os << "FoodEvent | " << static_cast<underlying_type>(type_) << std::endl;
            std::string str = os.str();
            os.str("");
            return str;
    }
};

class FoodMachine : public kuafu::StateMachine {
    public:
        FoodMachine(const std::string& name);

    public:
        virtual void Birth();

    public:
        kuafu::StateSharedPtr startup_;
        kuafu::StateSharedPtr loging_;
        kuafu::StateSharedPtr welcome_;

        kuafu::TransitionSharedPtr startup_loging_;
        kuafu::TransitionSharedPtr loging_welcome_;
        kuafu::TransitionSharedPtr loging_startup_;
        kuafu::TransitionSharedPtr welcome_startup_;
        kuafu::TransitionSharedPtr welcome_timeout_;
};

FoodMachine::FoodMachine(const std::string& name)
:StateMachine(name) {
}

void FoodMachine::Birth() {
    startup_ = kuafu::State::MakeState(*this, "startup");
    loging_ = kuafu::State::MakeState(*this, "loging");
    welcome_ = kuafu::State::MakeState(*this, "welcom", 5000);

    startup_loging_ = kuafu::Transition::MakeTransition("startup_loging",
                startup_,
                loging_,
                std::make_shared<kuafu::SimplePredicate<FoodEvent>>(FoodEventType::FET_LOGIN));
    loging_welcome_ = kuafu::Transition::MakeTransition("loging_welcome",
                loging_,
                welcome_,
                std::make_shared<kuafu::SimplePredicate<FoodEvent>>(FoodEventType::FET_LOGIN_OK));
    loging_startup_ = kuafu::Transition::MakeTransition("loging_startup",
                loging_,
                startup_,
                std::make_shared<kuafu::SimplePredicate<FoodEvent>>(FoodEventType::FET_LOGIN_FAILED));
    welcome_startup_ = kuafu::Transition::MakeTransition("welcome_startup",
                welcome_,
                startup_,
                std::make_shared<kuafu::SimplePredicate<FoodEvent>>(FoodEventType::FET_LOGOUT));
    welcome_timeout_ = kuafu::Transition::MakeTransition("welcome_timeout",
                welcome_,
                welcome_,
                std::make_shared<kuafu::TimeoutPredicate>(type_));
}
//----------------------------------------------------------
int main(int argc, char* agrv[]) {
    kuafu::Logger::init(kuafu::Logger::Level::LL_DEBUG_LOG,
                nullptr,
                "./test_fsm.log",
                false);

    kuafu::MachineSetSharedPtr machine_set = kuafu::MachineSet::MakeMachineSet();
    if (machine_set) {
        //set machine set and machine log
        machine_set->level = kuafu::ExternalLogger::Level::EL_DEBUG_LOG;
        machine_set->infolog_delegate = [&](std::ostringstream&& os) {
            INFO_LOG(os.str());
        };
        machine_set->debuglog_delegate = [&](std::ostringstream&& os) {
            DEBUG_LOG(os.str());
        };
        machine_set->errlog_delegate = [&](std::ostringstream&& os) {
            ERR_LOG(os.str());
        };
        machine_set->warninglog_delegate = [&](std::ostringstream&& os) {
            WARNING_LOG(os.str());
        };

        machine_set->StartBackground(500);

        //add foodmachine into machine set
        std::shared_ptr<FoodMachine> food_machine = kuafu::MakeMachine<FoodMachine>("food_machine");
        if (food_machine) {
            food_machine->SetStartState(food_machine->startup_);

            //set state's callback
            food_machine->startup_->OnEnter = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Enter " << state->GetName());
            };
            food_machine->startup_->OnExit = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Exit " << state->GetName());
            };

            food_machine->loging_->OnEnter = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Enter " << state->GetName());
            };
            food_machine->loging_->OnExit = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Exit " << state->GetName());
            };

            food_machine->welcome_->OnEnter = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Enter " << state->GetName());
            };
            food_machine->welcome_->OnExit = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& state) {
                INFO_LOG("Exit " << state->GetName());
            };

            //set transtition's callback
            food_machine->startup_loging_->OnTransition = [&](kuafu::MachineBase& machine,
                        const kuafu::StateSharedPtr& from_state,
                        kuafu::ITransitionSharedPtr transition,
                        kuafu::EventSharedPtr event,
                        const kuafu::StateSharedPtr& to_state) {
                INFO_LOG(transition->GetName()
                            << " | "
                            << from_state->GetName()
                            << " -> "
                            << to_state->GetName());
            };
            food_machine->loging_welcome_->OnTransition = [&](kuafu::MachineBase&,
                        const kuafu::StateSharedPtr& from_state,
                        kuafu::ITransitionSharedPtr transition,
                        kuafu::EventSharedPtr event,
                        const kuafu::StateSharedPtr& to_state) {
                INFO_LOG(transition->GetName()
                            << " | "
                            << from_state->GetName()
                            << " -> "
                            << to_state->GetName());
            };
            food_machine->loging_startup_->OnTransition = [&](kuafu::MachineBase&,
                        const kuafu::StateSharedPtr& from_state,
                        kuafu::ITransitionSharedPtr transition,
                        kuafu::EventSharedPtr event,
                        const kuafu::StateSharedPtr& to_state) {
                INFO_LOG(transition->GetName()
                            << " | "
                            << from_state->GetName()
                            << " -> "
                            << to_state->GetName());
            };
            food_machine->welcome_startup_->OnTransition = [&](kuafu::MachineBase&,
                        const kuafu::StateSharedPtr& from_state,
                        kuafu::ITransitionSharedPtr transition,
                        kuafu::EventSharedPtr event,
                        const kuafu::StateSharedPtr& to_state) {
                INFO_LOG(transition->GetName()
                            << " | "
                            << from_state->GetName()
                            << " -> "
                            << to_state->GetName());
            };
            food_machine->welcome_timeout_->OnTransition = [&](kuafu::MachineBase&,
                        const kuafu::StateSharedPtr& from_state,
                        kuafu::ITransitionSharedPtr transition,
                        kuafu::EventSharedPtr event,
                        const kuafu::StateSharedPtr& to_state) {
                INFO_LOG(transition->GetName()
                            << " | "
                            << from_state->GetName()
                            << " -> "
                            << to_state->GetName());
            };


            machine_set->Enqueue(std::make_shared<kuafu::MachineOperationEvent>(
                            kuafu::MachineOperator::MO_ADD,
                            food_machine));

            machine_set->Enqueue(std::make_shared<FoodEvent>(
                            FoodEventType::FET_LOGIN,
                            food_machine));
            machine_set->Enqueue(std::make_shared<FoodEvent>(
                            FoodEventType::FET_LOGIN_OK,
                            food_machine));
            machine_set->Enqueue(std::make_shared<FoodEvent>(
                            FoodEventType::FET_LOGOUT,
                            food_machine));
        }
    }

    getchar();

    return 0;
}
