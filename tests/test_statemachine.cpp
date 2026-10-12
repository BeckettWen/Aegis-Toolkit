// this is the test file of the state machine

#include "../StateMachine.hpp"
#include <gtest/gtest.h>

namespace {
enum class State { Idle, Running, Unknown };
enum class Event { Start, Stop };

using Machine = Aegis_stateMachine::StateMachine<State, Event>;
}

class StateMachineTest : public ::testing::Test {
protected:
    // Named variables are needed because your API takes State&.
    State idle = State::Idle;
    State running = State::Running;
    State unknown = State::Unknown;

    int starts = 0;
    int stops = 0;
    std::vector<Event> actions;

    Machine machine{idle};

    void RegisterCycle() {
        machine.Register_Rules_With_Events(
            idle,
            [this] {
                ++starts;
                actions.push_back(Event::Start);
            },
            Event::Start,
            running
        );

        machine.Register_Rules_With_Events(
            running,
            [this] {
                ++stops;
                actions.push_back(Event::Stop);
            },
            Event::Stop,
            idle
        );
    }
};

TEST_F(StateMachineTest, ExecutesActionAndRejectsMissingRule) {
    RegisterCycle();

    auto success = machine.Transition(idle);
    ASSERT_TRUE(success.has_value());
    EXPECT_EQ(starts, 1);
    EXPECT_EQ(stops, 0);

    auto failure = machine.Transition(unknown);
    ASSERT_FALSE(failure.has_value());
    EXPECT_EQ(failure.error(), "State Rule Not Found");
    EXPECT_EQ(starts, 1);
    EXPECT_EQ(stops, 0);
}

TEST_F(StateMachineTest, UnregisteredInitialStateReturnsError) {
    auto result = machine.Transition(idle);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "State Rule Not Found");
}

TEST_F(StateMachineTest, RegistrationDoesNotExecuteActions) {
    RegisterCycle();

    EXPECT_EQ(starts, 0);
    EXPECT_EQ(stops, 0);
    EXPECT_TRUE(actions.empty());
}

TEST_F(StateMachineTest, EmptyActionIsAllowed) {
    machine.Register_Rules_With_Events(
        idle, {}, Event::Start, running
    );

    auto result = machine.Transition(idle);
    EXPECT_TRUE(result.has_value());
}

TEST_F(StateMachineTest, ExecutesCycleInOrder) {
    RegisterCycle();

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(machine.Transition(idle).has_value());
        ASSERT_TRUE(machine.Transition(running).has_value());
    }

    EXPECT_EQ(starts, 3);
    EXPECT_EQ(stops, 3);

    const std::vector<Event> expected{
        Event::Start, Event::Stop,
        Event::Start, Event::Stop,
        Event::Start, Event::Stop
    };
    EXPECT_EQ(actions, expected);
}

TEST_F(StateMachineTest, RejectsRuleForNonCurrentState) {
    RegisterCycle();

    // Current state is Idle, so Running's rule must not execute.
    auto result = machine.Transition(running);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(starts, 0);
    EXPECT_EQ(stops, 0);
    EXPECT_TRUE(actions.empty());
}

TEST_F(StateMachineTest, TransitionChangesWhichSourceStateIsAllowed) {
    RegisterCycle();

    ASSERT_TRUE(machine.Transition(idle).has_value());

    // After Idle -> Running, Idle is no longer the current state.
    auto repeated = machine.Transition(idle);
    EXPECT_FALSE(repeated.has_value());
    EXPECT_EQ(starts, 1);

    // Running's rule should now be allowed.
    ASSERT_TRUE(machine.Transition(running).has_value());
    EXPECT_EQ(stops, 1);
}

TEST_F(StateMachineTest, StoreStateChangesWhichSourceStateIsAllowed) {
    RegisterCycle();

    ASSERT_TRUE(machine.Store_State(running).has_value());

    // Store_State must affect behavior, not just return success.
    auto old_state = machine.Transition(idle);
    EXPECT_FALSE(old_state.has_value());
    EXPECT_EQ(starts, 0);

    ASSERT_TRUE(machine.Transition(running).has_value());
    EXPECT_EQ(stops, 1);
}

TEST_F(StateMachineTest, MissingRuleDoesNotPreventValidTransition) {
    RegisterCycle();

    auto missing = machine.Transition(unknown);
    ASSERT_FALSE(missing.has_value());
    EXPECT_TRUE(actions.empty());

    ASSERT_TRUE(machine.Transition(idle).has_value());
    EXPECT_EQ(starts, 1);
    EXPECT_EQ(stops, 0);
}

TEST_F(StateMachineTest, SelfTransitionCanBeRepeated) {
    machine.Register_Rules_With_Events(
        idle, [this] { ++starts; }, Event::Start, idle
    );

    for (int i = 0; i < 3; ++i) {
        ASSERT_TRUE(machine.Transition(idle).has_value());
    }

    EXPECT_EQ(starts, 3);
}
