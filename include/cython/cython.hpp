#pragma once
#include "fsm.hpp"
using namespace std;

struct Cython
{
    vector <pair <string, string>> variables;
    STATE ("begin") begin_state;
    
    Context ctx
    {
        .parent = nullptr,
        .declaredVariables = variables,
        .state = &begin_state
    };
    
    string process_text (string const& str)
    {
        for (auto i = str.begin(); i < str.end(); ++i)
            ctx.process (i);
        
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
    
    ~Cython ()
    {
        ctx.result.clear ();
//        delete ctx.state;
    }
    
};
