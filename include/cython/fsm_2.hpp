#pragma once
using namespace std;
#include <const_str/const_str.hpp>
#include <ph_macros/ph_macros.hpp>
#define INDENTION 4
#define SS2(x, y) EVAL (IF_ELSE (PH_MACRO_$token_is_string (x, "begin"))('b', 'e', 'g', 'i', 'n')())
#define SS(x) IF_ELSE (PH_MACRO_$token_is_string (x, "begin")) ('b', 'e', 'g', 'i', 'n') ()

#define BEGIN 'b', 'e', 'g', 'i', 'n'

#define DONE 'd', 'o', 'n', 'e'

using iter = string::const_iterator;





template <char...>
struct State2;




struct Context2 {
    using variable_iter = vector <pair <string, string>>::iterator;
    
    
    Context2* parent {nullptr};
    vector <pair <string, string>>& declaredVariables;
    State2<>* state {nullptr};
    vector <Context2*> children;
    stack <char> bracketStack;
    
    inline static string result = "";
    string variable;
    string value;
    string potential;
    string firstint;
    string secondint;
    string intvariable;
    string loop;
    bool looping {false};
    int max_indention = INDENTION;

    pair <bool, int> indention {false, 0}; //if already indented and if not, how much
    
    void process (iter it);
    
    
    auto findVariable (string const& name) -> optional <variable_iter>
    {
        auto i = declaredVariables.begin ();
        for (; i < declaredVariables.end (); ++i)
        {
            if (i -> first == name)
                return {i};
        }
        return {};
    }
};


template <>
struct State2 <> {
    void process (iter it, Context2& ctx) {
        if (ctx.children.empty ()) {
            _process (it, ctx);
        } else
        {
            for (auto& o : ctx.children)
                o -> process (it);
        }
    }
    virtual void _process (iter it, Context2& ctx) {
        throw runtime_error ("");
    }
    using variable_iter = vector <pair <string, string>>::iterator;
    bool hasParent (Context2& ctx) const;
    string& variable (Context2& ctx);
    string& value (Context2& ctx);
    string& result (Context2& ctx);
    string& potential (Context2& ctx);
    void removeFromParent (Context2& ctx);
    template <class T> void transition (Context2& ctx);
    template <class T> Context2& addChildContext (Context2& ctx);
    State2 <> * parent (Context2& ctx);
    void declare (string const& var, string const& val, Context2& ctx);
    optional <string> declared (string const&, Context2& ctx);
    virtual void addResultFromChild (string const& res, Context2& ctx);
    void reset (Context2& ctx);
    virtual void reset_hasNoParent (Context2& ctx){}
    virtual void reset_hasParent (Context2& ctx){}
    virtual string trans(){
//        throw runtime_error ("");
        return "";
    }
    string transi (Context2& ctx);
    void chainChildren (iter i, Context2& ctx);
    void clear (Context2& ctx);
};



void Context2::process (iter it) {
    state -> process (it, *this);
}

void State2<>::clear (Context2& ctx)
{
    ctx.variable.clear();
    ctx.value.clear();
    ctx.potential.clear();
    ctx.firstint.clear ();
    ctx.secondint.clear ();
    ctx.intvariable.clear ();
    ctx.loop.clear ();
}


void State2<>::declare (string const& variable_name, string const& variable_value, Context2& ctx) {
//    throw runtime_error ("");
    auto declared = ctx.declaredVariables.begin ();
    for (; declared < ctx.declaredVariables.end (); ++declared) {
        if (declared -> first == variable_name) {
            declared -> second = variable_value;
            return;
        }
    }
    ctx.declaredVariables.emplace_back (variable_name, variable_value);
}
optional <string> State2<>::declared (string const& p, Context2& ctx) {
    throw runtime_error ("");
    for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return d->second;
        }
    }
    return {};
}

State2<>* State2<>::parent (Context2& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr or ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    return ctx.parent -> state;
}

void State2<>::reset (Context2& ctx){
    if (not hasParent(ctx)) {
        reset_hasNoParent(ctx);
    } else {
        reset_hasParent(ctx);
    }
}


void State2<>::addResultFromChild (string const& res, Context2& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr || ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    ctx.parent -> state -> addResultFromChild (res, *ctx.parent);
}


bool State2<>::hasParent (Context2& ctx) const {
    return ctx.parent != nullptr and ctx.parent->state != nullptr;
}

void State2<>::removeFromParent (Context2& ctx) {
#if defined (Debug)
    for (int i = 0; i < ctx.parent->children.size (); ++i) {
        if (ctx.parent->children[i] == &ctx) {
//            cout << "removing child context from parent context" << endl;
            ctx.parent -> children.erase (ctx.parent->children.begin() + i);
//            ctx.parent -> children.
            delete this;
            return;
        }
    }
    throw runtime_error ("");
#else
    for (auto cont = ctx.parent -> children.begin(); cont < ctx.parent -> children.end(); ++cont) {
        if (*cont == &ctx) {
//            cout << "removing child context from parent context" << endl;
            ctx.parent -> children.erase (cont);
            break;
        }
    }
#endif
    
}




template <class Q>
Context2& State2<>::addChildContext (Context2& ctx) {
//    cout << endl;
    State2<>* childState = new Q;
    Context2* childContext = new Context2 {&ctx, ctx.declaredVariables, childState};
    childContext -> max_indention = ctx.max_indention + INDENTION;
//    childState -> context = childContext;
    ctx.children.push_back (childContext);
    return *childContext;
#undef INDENTION
}

string& State2<>::variable (Context2& ctx) {
    return ctx.variable;
}
string& State2<>::value (Context2& ctx) {
    return ctx.value;
}
string& State2<>::result (Context2& ctx) {
    return ctx.result;
}
string& State2<>::potential (Context2& ctx) {
    return ctx.potential;
}

string State2<>::transi (Context2& ctx) {
    if (hasParent (ctx)){
        return ctx.parent->state->transi(*ctx.parent) + "::" + trans();
    } else
    {
        return trans();
    }
}
void State2<>::chainChildren (iter i, Context2& ctx) {
#if defined (Debug)
    if (ctx.children.empty ())
        throw runtime_error ("");
#endif
    for (auto& c : ctx.children)
    {
        c->process(i);
    }
}
template <class T>
void State2<>::transition (Context2& ctx) {
    
//    cout << transi(ctx) << " -> ";

    
    ctx.state = new T;
    
    
//    cout << ctx.state->transi(ctx) << endl;
//    cout << ctx.result << endl;
}


template <>
struct State2 <BEGIN> : State2 <> {
    virtual void _process (iter i, Context2& ctx){
        
        if (*i == DECLPASTE)
        {
            potential (ctx) += *i;
//            transition <State <STR ("T"), STATE ("$")>> (ctx);

        }
//        else if (*i == '#')
//        {
//            potential (ctx) += '#';
////            transition <State <STR ("T"), STATE ("#")>> (ctx);
//
//        }
//        else if (*i == '@')
//        {
//            potential (ctx) += '@';
////            transition <State <STR ("T"), STATE ("@")>> (ctx);
//
//        }
//        else
//        {
            if (hasParent (ctx))
            {
                State2 <>::addResultFromChild (string {*i}, ctx);
                
            } else
            {
                ctx.result += *i;
            }
//        }
       

    }

};

template <>
struct State2 <DONE> : State2 <BEGIN> {};

template <char c>
struct State2 <c, '(', ')'> : State2 <>
{
    
    virtual void _process (iter i, Context2& ctx)
    {
        ctx.potential += *i;
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            finish (ctx);
//            template transition <State <STR ("T(...){"), STATE ("$(0 i 5){")>> (ctx);
//            TRANSITION ("$(0 i 5){");
        } else if (*i == ' ')
        {

        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else
        {
            if (hasParent (ctx))
            {
                addResultFromChild (ctx.potential, ctx);
                clear (ctx);
                transition <State2 <BEGIN>> (ctx);
//                TRANSITION ("begin");
//                removeFromParent (ctx);
            } else
            {
                ctx.result += ctx.potential;
                clear (ctx);
                transition <State2 <BEGIN>> (ctx);
            }
        }
    }
    
    void finish (Context2& ctx)
    {
        
    }
    
};


template <char c>
struct State2 <c, '('> : State2 <>
{
    
    virtual void _process (iter i, Context2& ctx)
    {
        if (*i == ')')
        {
            ctx.potential += *i;
            
            if constexpr (c == '$')
                transition <State2 <c, '(', ')'>> (ctx);
            
            else if constexpr (c == '@')
                transition <State2 <c, '(', ')'>> (ctx);
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else if (*i == DECLPASTE)
        {
            transition <State2 <'$'>> (ctx);

        } else if (*i == '@')
        {
            transition <State2 <'@'>> (ctx);
            
        } else if (isnumber (*i))
        {
            ctx.potential += *i;
            ctx.firstint = *i;
            
            if constexpr (c == '$')
            {
//                template transition <State <STR ("T()"), STATE ("$(0")>> (ctx);
                transition <State2 <'$', '(', '0'>> (ctx);
                
            } else if constexpr (c == '@')
            {
//                template transition <State <STR ("T()"), STATE ("@(x")>> (ctx);
            }
        } else
        {
            ctx.variable += *i;
            ctx.potential += *i;
        }
    }
    
    virtual void addResultFromChild (string const& res, Context& ctx){
        ctx.variable += res;
    }
};

template <char c>
struct State2 <c> : State2 <>
{
    
    void change_state (Context2& ctx)
    {
        if constexpr (c == '@')
        {
            transition <State2 <c, '@'>> (ctx);
        }

        else if constexpr (c == '$')
        {
            transition <State2 <c, '$'>> (ctx);
        }

        else if constexpr (c == '#')
        {
            transition <State2 <c, '#'>> (ctx);
        }
    }
   
    void _process (iter i, Context2& ctx){
        
        ctx.potential += *i;
        
        if (*i == '(')
        {
            if constexpr (c == '@')
            {
                change_state (ctx);
                
            } else if constexpr (c == '$')
            {
                change_state (ctx);
                
            } else if constexpr (c == '#')
            {
                reset (ctx);
            }
           
        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            
            if constexpr (c == '$')
            {
//                template transition <State <STR ("T(...){"), STATE ("${")>> (ctx);
                
            } else if constexpr (c == '@')
            {
                reset (ctx);
                
            } else if constexpr (c == '#')
            {
//                template transition <STATE ("#{")> (ctx);
            }
            
        } else if (*i == ' ')
        {
            if constexpr (c != '#')
                ctx.potential += ' ';
            
        } else if (*i == '\n')
        {
            if constexpr (c != '#')
                ctx.potential += '\n';
            
        } else
        {
            reset (ctx);
        }
    }
    
    void reset (Context2& ctx)
    {
        if (hasParent (ctx))
        {
            addResultFromChild (ctx.potential, ctx);
            clear (ctx);
            
            if (ctx.looping)
            {
                transition <State2 <BEGIN>> (ctx);
                
            } else
            {
                removeFromParent (ctx);
            }

        } else
        {
            ctx.result += ctx.potential;
            clear (ctx);
            transition <State2 <BEGIN>> (ctx);
        }
    }
};
