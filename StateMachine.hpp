// this is the state machine tool

#pragma once

#include <vector>
#include <queue>
#include <expected>
#include <type_traits>
#include <functional>
#include <any>

namespace Aegis_stateMachine{

    std::any stored_State{};

    // this is the general API you use to set the initial state
    template<typename type>
        requires std::is_enum_v<type>
    std::expected<void, std::string> InitializeState(type& initial_state){
        try{
            stored_State = std::ref(initial_state);
        }
        catch(const std::exception& e){
            return std::unexpected<std::string>(e.what());
        }
        
        return {};
    }

    // retrieve the data inside the 'Any type enumeration' 
    template<typename type>
    type& retrieve_value(){
        return std::any_cast<std::reference_wrapper<type>>(stored_State).get();
    }

    template<typename type>
    class StateMachine{
        
    };
}