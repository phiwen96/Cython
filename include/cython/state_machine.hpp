#pragma once
#include <ph_type_list/type_list.hpp>
using namespace std;



template <typename... states>
struct state_machine
{
    tuple <states...> available_states;
    variant <states...> current_state;
    
    state_machine ();
    

    template <class state>
    auto transition_to (state&& s) -> decltype (auto) {
        cout << "machine: transitioning to state " << s << endl;
        using type = decay_t <state>;
        current_state = get <type> (current_state);
    }
    

    auto handle (auto&& e) -> decltype (auto) {
        cout << "machine: handling event " << e << endl;
        auto pass_event_to_state = [e, this] (auto&& state) {
            state.handle (e).execute (*this);
        };
        visit (pass_event_to_state, current_state);
    }
    
    
};


template <typename... states>
state_machine<states...>::state_machine () : current_state {get <0> (available_states)} {
    
}

template <typename state>
struct transition_to
{
    template <typename machine>
    auto execute (machine&& m) -> decltype (auto) {
        m.template transition_to (state {});
    }
};




