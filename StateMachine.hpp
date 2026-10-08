// this is the state machine tool
// Notice that although these tools are separated in different files
// the version is all the same

#pragma once

#include <expected>
#include <type_traits>
#include <functional>
#include <any>
#include <string>
#include <map>
#include <vector>
#include <tuple>

#include "VersionInfo.h"

using event_type_General = std::function<void()>;

namespace Aegis_stateMachine{


    template<typename State, typename Events>
    class StateMachine{
        // stores the current state
        std::vector<State> state_set;
        State current_state;

        using transition_rule = std::tuple<Events, State, event_type_General>;
        std::unordered_map<State, transition_rule> registry;

        StateMachine<State, Events>(const StateMachine<State, Events>&) = delete;
        ~StateMachine<State, Events>(){};

        std::expected<void, std::string> Store_State(State& current_state_input){}

        void Register_Event(State& state, event_type_General event) {
            // register the specific event to the state
        }

        std::expected<void, std::string> Transition(State& state) {
            

        }
    };
}