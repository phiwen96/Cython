#ifndef CYTHON_2_HPP
#define CYTHON_2_HPP
#include "fsm_2.hpp"
using namespace std;


struct Cython2
{
    vector <pair <string, string>> variables;
    
//    STATE ("begin") begin_state;
    State2 <BEGIN> begin_state;
    
    Context2 ctx
    {
        .parent = nullptr,
        .declaredVariables = variables,
        .state = &begin_state
    };
    
    string process_text (string const& str)
    {
        
//        return "";
        ctx.result.clear ();
        
        for (auto i = str.begin(); i < str.end(); ++i)
            ctx.process (i);
//        ctx.result.clear ();
  
        if (State2 <DONE>* d = dynamic_cast<State2 <DONE>*>(ctx.state))
        {
            
        } else
        {
            ctx.result += ctx.potential;
        }
        return ctx.result;
    }
    
    void clear_variables ()
    {
        variables.clear ();
    }
    
    void add_variable (string&& variable_name, string&& variable_value)
    {
        for (auto& i : variables)
        {
            if (i.first == variable_name)
            {
                swap (i.second, variable_value);
                return;
            }
        }
        variables.emplace_back (move (variable_name), move (variable_value));
    }
    
    auto get_variables () const -> vector <pair <string, string>> const&
    {
        return variables;
    }
    
    ~Cython2 ()
    {
        ctx.result.clear ();
//        delete ctx.state;
    }
    
};
#endif
