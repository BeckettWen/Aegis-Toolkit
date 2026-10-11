// this is the test file of the state machine

#include "../StateMachine.hpp"
#include <gtest/gtest.h>

namespace {
    enum class State { Idle, Running, Unknown };
    enum class Event { Start };
}

TEST(StateMachineTest, ExecutesActionAndRejectsMissingRule) {
    State idle = State::Idle;
    State running = State::Running;
    State unknown = State::Unknown;

    Aegis_stateMachine::StateMachine<State, Event> machine(idle);

    int action_count = 0;
    machine.Register_Rules_With_Events(
        idle,
        [&action_count] { ++action_count; },
        Event::Start,
        running
    );

    auto success = machine.Transition(idle);
    ASSERT_TRUE(success.has_value());
    EXPECT_EQ(action_count, 1);

    auto failure = machine.Transition(unknown);
    ASSERT_FALSE(failure.has_value());
    EXPECT_EQ(failure.error(), "State Rule Not Found");
    EXPECT_EQ(action_count, 1); // Missing rule must not execute an action.
}
