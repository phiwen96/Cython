#pragma once
using namespace std;
#include <const_str/const_str.hpp>
#include <ph_macros/ph_macros.hpp>
#define INDENTION 4


#define BEGIN 'b', 'e', 'g', 'i', 'n'

#define DONE 'd', 'o', 'n', 'e'
#define LOOP 'l', 'o', 'o', 'p'

#define DECLPASTE '$'
#define DECLARE '@'
#define COMMENT '#'

#define KEYS DECLPASTE, DECLARE, COMMENT

using iter = string::const_iterator;

template <char c, char... r>
auto _if_is_key (iter d, auto&& func) -> decltype (auto){
    if (c == *d)
    {
        func ();
        return true;
    }
    else if constexpr (sizeof... (r) == 0)
        return false;
    else
        return _if_is_key <r...> (d, forward <decltype (func)> (func));
}

template <class F>
auto is_key (iter c, F func) -> decltype (auto) {
    return _if_is_key <KEYS> (c, forward <F> (func));
}
 
#define BAS

#define NO_PASTE 'n', 'o', ' ', 'p', 'a', 's', 't', 'e'

#define SUCCESS 's', 'u', 'c', 'c', 'e', 's', 's'
#define FAIL 'f', 'a', 'i', 'l'



#define IF_KEY(...) if (*i == DECLPASTE) {__VA_ARGS__<DECLPASTE> (i, ctx);} else if (*i == DECLARE) {__VA_ARGS__<DECLARE> (i, ctx);} else if (*i == COMMENT) {__VA_ARGS__<COMMENT> (i, ctx);}


#define IF_KEY2(...) __builtin_choose_expr (*i == DECLPASTE, __VA_ARGS__, (__builtin_choose_expr (*i == DECLARE, __VA_ARGS__, (__builtin_choose_expr (*i == COMMENT, __VA_ARGS__, BOOST_PP_EMPTY ())))))
#define IF_CHILD if (hasParent (ctx))
#define PUSH_PARENT(...) State2 <>::addResultFromChild (__VA_ARGS__, ctx);
#define ADD_POTENTIAL ctx.potential += *i;

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
    
    template <class T>
    void transition_to ()
    {
        state = new T;
    }
    
    
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
    
    void declare_variable (string&& name, string&& value)
    {
        
        auto declared = declaredVariables.begin ();
        for (; declared < declaredVariables.end (); ++declared) {
            if (declared -> first == name) {
                swap (declared -> second, value);
                return;
            }
        }
        declaredVariables.emplace_back ((string&&) name, (string&&) value);
    }
    
    void declare_variable (string const& name, string const& value)
    {
        auto declared = declaredVariables.begin ();
        for (; declared < declaredVariables.end (); ++declared) {
            if (declared -> first == name) {
                declared -> second = value;
                return;
            }
        }
        declaredVariables.emplace_back (name, value);
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
    void removeFromParent (Context2& ctx);
    template <char... c> void transition (Context2& ctx);
    template <char... c> Context2& addChildContext (Context2& ctx);
    State2 <> * parent (Context2& ctx);
    void declare (string const& var, string const& val, Context2& ctx);
    optional <string> declared (string const&, Context2& ctx);
    virtual void addResultFromChild (string&& res, Context2& ctx);
    void reset (Context2& ctx);
    virtual void reset_hasNoParent (Context2& ctx){}
    virtual void reset_hasParent (Context2& ctx){}
    void clear (Context2& ctx);
    virtual ~State2 () {
        
    }
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


void State2<>::addResultFromChild (string&& res, Context2& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr || ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    ctx.parent -> state -> addResultFromChild ((string&&) res, *ctx.parent);
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




template <char... c>
Context2& State2<>::addChildContext (Context2& ctx) {
//    cout << endl;
    State2<>* childState = new State2 <c...>;
    Context2* childContext = new Context2 {&ctx, ctx.declaredVariables, childState};
    childContext -> max_indention = ctx.max_indention + INDENTION;
//    childState -> context = childContext;
    ctx.children.push_back (childContext);
    return *childContext;

}



template <char... c>
void State2<>::transition (Context2& ctx) {
    
//    cout << transi(ctx) << " -> ";

    
    ctx.state = new State2 <c...>;
    
    
//    cout << ctx.state->transi(ctx) << endl;
//    cout << ctx.result << endl;
}



template <char...>
struct _Done
{
    using type = State2 <DONE>;
};

template <>
struct _Done <DECLARE>
{
    using type = State2 <DONE, NO_PASTE>;
};

template <>
struct _Done <COMMENT>
{
    using type = State2 <DONE, NO_PASTE>;
    
};


template <char... c>
using Done = typename _Done <c...>::type;

template <char...>
struct _get_next_transition;



template <char... c>
using get_next_transition = typename _get_next_transition <c...>::type;