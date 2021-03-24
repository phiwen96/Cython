#pragma once
using namespace std;
#include <const_str/const_str.hpp>
#include <ph_macros/ph_macros.hpp>
#define INDENTION 4
#define SS2(x, y) EVAL (IF_ELSE (PH_MACRO_$token_is_string (x, "begin"))('b', 'e', 'g', 'i', 'n')())
#define SS(x) IF_ELSE (PH_MACRO_$token_is_string (x, "begin")) ('b', 'e', 'g', 'i', 'n') ()

#define BEGIN 'b', 'e', 'g', 'i', 'n'

#define DONE 'd', 'o', 'n', 'e'

#define DECLPASTE '$'

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
    void removeFromParent (Context2& ctx);
    template <char... c> void transition (Context2& ctx);
    template <char... c> Context2& addChildContext (Context2& ctx);
    State2 <> * parent (Context2& ctx);
    void declare (string const& var, string const& val, Context2& ctx);
    optional <string> declared (string const&, Context2& ctx);
    virtual void addResultFromChild (string const& res, Context2& ctx);
    void reset (Context2& ctx);
    virtual void reset_hasNoParent (Context2& ctx){}
    virtual void reset_hasParent (Context2& ctx){}
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




template <char... c>
Context2& State2<>::addChildContext (Context2& ctx) {
//    cout << endl;
    State2<>* childState = new State2 <c...>;
    Context2* childContext = new Context2 {&ctx, ctx.declaredVariables, childState};
    childContext -> max_indention = ctx.max_indention + INDENTION;
//    childState -> context = childContext;
    ctx.children.push_back (childContext);
    return *childContext;
#undef INDENTION
}



template <char... c>
void State2<>::transition (Context2& ctx) {
    
//    cout << transi(ctx) << " -> ";

    
    ctx.state = new State2 <c...>;
    
    
//    cout << ctx.state->transi(ctx) << endl;
//    cout << ctx.result << endl;
}


template <char... parent>
struct State2 <'{', '\n', parent...> : State2 <parent...>
{
    using self = State2 <parent..., '\n'>;
    using Parent = State2 <parent...>;
    virtual void _process (iter i, Context2& ctx)
    {
        if (*i == '\n')
        {
            ctx.value += *i;
            ctx.indention.first = false;
            
        } else if (*i == ' ' and ctx.indention.first == false)
        {
            ++ctx.indention.second;
//            Parent::template transition<State <STR ("{\nx"), self>>(ctx);
            Parent::template transition<'{', '\n', 'x', parent...>(ctx);
            
        } else
        {
            Parent::_process (i, ctx);
        }
    }
};

template <char... parent>
struct State2 <'{', '\n', 'x', parent...> : State2 <parent...>
{
    using Parent = State2 <parent...>;
    virtual void _process (iter i, Context2& ctx)
    {
        if (*i == ' ')
        {
            ctx.potential += ' ';
            if (++ctx.indention.second == ctx.max_indention)
            {
                ctx.indention.second = 0;
                ctx.indention.first = true;
                Parent::template transition <parent...> (ctx);
            }
        } else
        {
            ctx.indention.second = 0;
            Parent::template transition <parent...> (ctx);
            Parent:: _process (i, ctx);
        }
    }
};




template <>
struct State2 <BEGIN> : State2 <> {
    virtual void _process (iter i, Context2& ctx){
        
        if (*i == DECLPASTE)
        {
            ctx.potential += *i;
//            transition <State <STR ("T"), STATE ("$")>> (ctx);
            transition <'$'> (ctx);

        }
        else if (*i == '#')
        {
            ctx.potential += '#';
            transition <'#'> (ctx);

//            transition <State <STR ("T"), STATE ("#")>> (ctx);

        }
        else if (*i == '@')
        {
            ctx.potential += '@';
            transition <'@'> (ctx);

//            transition <State <STR ("T"), STATE ("@")>> (ctx);

        }
        else
        {
            if (hasParent (ctx))
            {
                State2 <>::addResultFromChild (string {*i}, ctx);
                
            } else
            {
                ctx.result += *i;
            }
        }
       

    }

};

template <>
struct State2 <DONE> : State2 <BEGIN> {};

template <char c>
struct State2 <c, '(', ')', '{', DONE> : State2 <DONE>
{
    virtual void _process (iter i, Context2& ctx){
        if (*i == '\n')
        {
            ctx.potential += '\n';
            transition <DONE> (ctx);
        } else
        {
            transition <DONE> (ctx);
            State2 <DONE>::_process (i, ctx);
        }
    }
};

template <char c>
struct State2 <c, '{'> : State2 <>
{
    inline static constexpr bool parent_is_loop_type = false;//is_same_v <T, STATE ("$(0 i 5){")>;
//    using self = State2 <c, '(', ')', '{'>;
    
    virtual void _process (iter i, Context2& ctx)
    {
//        cout << "yo" << endl;
        if constexpr (not parent_is_loop_type)
        {
            if (*i == DECLPASTE)
            {
                addChildContext <'$'> (ctx).potential = *i;
                return;
                
            } else if (*i == '@')
            {
                addChildContext <'@'> (ctx).potential = *i;
                return;
                
            } else if (*i == '#')
            {
                addChildContext <'#'> (ctx).potential = *i;
                return;
                
            }
        }
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                finish (ctx);
                
            } else
            {
                ctx.potential += '}';
                ctx.value += *i;
            }
            
        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            ctx.potential += '{';
            ctx.value += *i;
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
//            transition <State <STR ("{\n"), self>> (ctx);
            transition <'{', '\n', c, '{'> (ctx);

        } else
        {
            ctx.value += *i;
            ctx.potential += *i;
        }
    }
    virtual void addResultFromChild (string const& res, Context2& ctx)
    {
        if constexpr (parent_is_loop_type)
        {
            ctx.loop += res;
            return;
            
        } else
        {
            ctx.value += res;
            ctx.potential += res;
        }
    }
    
    void finish (Context2& ctx);
};

template <>
void State2 <'$', '{'>::finish (Context2& ctx) {
    auto declared = ctx.findVariable (ctx.value);
    if (declared)
    {
        if (hasParent (ctx))
        {
//            cout << "lol" << endl;
//            cout << ":" << declared.value () -> second << endl;
            State2 <>::addResultFromChild (declared.value()->second, ctx);
            clear (ctx);
            
            if (ctx.looping)
            {
//                TRANSITION ("begin")
                transition <BEGIN> (ctx);
                
            } else
            {
                removeFromParent (ctx);
            }
        } else
        {
            ctx.result += declared.value ()->second;
            clear (ctx);
            transition <DONE> (ctx);

//            TRANSITION ("done")
        }
        
    } else
   {
       string warning = "variable \"" + ctx.value + "\" pasted but it has not yet been declared!";
       throw runtime_error (warning);
   }
};


template <char c>
struct State2 <c, '(', ')', '{'> : State2 <>
{
    inline static constexpr bool parent_is_loop_type = false;//is_same_v <T, STATE ("$(0 i 5){")>;
    using self = State2 <c, '(', ')', '{'>;
    
    virtual void _process (iter i, Context2& ctx)
    {
        if constexpr (not parent_is_loop_type)
        {
            if (*i == DECLPASTE)
            {
//                cout << "hi" << endl;
                addChildContext <'$'> (ctx).potential = *i;
                return;
                
            } else if (*i == '@')
            {
                addChildContext <'@'> (ctx).potential = *i;
                return;
                
            } else if (*i == '#')
            {
                addChildContext <'#'> (ctx).potential = *i;
                return;
                
            }
        }
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                finish (ctx);
                
            } else
            {
                ctx.potential += '}';
                ctx.value += *i;
            }
            
        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            ctx.potential += '{';
            ctx.value += *i;
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
//            transition <State <STR ("{\n"), self>> (ctx);
            transition <'{', '\n', c, '(', ')', '{'> (ctx);

        } else
        {
            ctx.value += *i;
            ctx.potential += *i;
        }
    }
    virtual void addResultFromChild (string const& res, Context2& ctx)
    {
        if constexpr (parent_is_loop_type)
        {
            ctx.loop += res;
            return;
            
        } else
        {
//            cout << "hi2" << endl;
            ctx.value += res;
            ctx.potential += res;
        }
    }
    
    void finish (Context2& ctx);
};
    

template <>
void State2<'$', '(', ')', '{'>::finish (Context2& ctx) {
    
    if (auto found = ctx.findVariable (ctx.variable); found)
    {
        found.value()->second = ctx.value;
        
    } else
    {
        ctx.declaredVariables.emplace_back (ctx.variable, ctx.value);
    }
    
    if (ctx.value.back () == '\n')
    {
        ctx.value.pop_back ();
    }
    if (hasParent(ctx))
    {
        addResultFromChild (ctx.value, ctx);
        clear (ctx);
        
        if (ctx.looping)
        {
            transition <BEGIN> (ctx);
            
        } else
        {
            removeFromParent (ctx);
        }

    } else
    {
        ctx.result += ctx.value;
        clear (ctx);
        transition <DONE> (ctx);
    }
}
    

template <>
void State2<'@', '(', ')', '{'>::finish (Context2& ctx) {
    
    if (ctx.value.back () == '\n')
    {
        ctx.value.pop_back ();
    }
    
    declare (ctx.variable, ctx.value, ctx);
    clear (ctx);
    transition <'@', '(', ')', '{', DONE> (ctx);

//    TRANSITION ("T(...){ done")
}

template <char c>
struct State2 <c, '(', ')'> : State2 <>
{
    
    virtual void _process (iter i, Context2& ctx)
    {
        ctx.potential += *i;
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
//            finish (ctx);
            transition <c, '(', ')', '{'> (ctx);
//            template transition <State <STR ("T(...){"), STATE ("$(0 i 5){")>> (ctx);
//            TRANSITION ("$(0 i 5){");
        } else if (*i == ' ')
        {
            ctx.potential += '\n';
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else
        {
            if (hasParent (ctx))
            {
                addResultFromChild (ctx.potential, ctx);
                clear (ctx);
                transition <BEGIN> (ctx);
//                TRANSITION ("begin");
//                removeFromParent (ctx);
            } else
            {
                ctx.result += ctx.potential;
                clear (ctx);
                transition <BEGIN> (ctx);
            }
        }
    }
    
    void finish (Context2& ctx);
    
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
                transition <c, '(', ')'> (ctx);
            
            else if constexpr (c == '@')
                transition <c, '(', ')'> (ctx);
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else if (*i == DECLPASTE)
        {
            transition <'$'> (ctx);
            addChildContext <'$'> (ctx);

        } else if (*i == '@')
        {
            addChildContext <'@'> (ctx);

        } else if (isnumber (*i))
        {
            ctx.potential += *i;
            ctx.firstint = *i;
            
            if constexpr (c == '$')
            {
//                template transition <State <STR ("T()"), STATE ("$(0")>> (ctx);
                transition <'$', '(', '0'> (ctx);
                
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
    
    virtual void addResultFromChild (string const& res, Context2& ctx){
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
            transition <c, '('> (ctx);
        }

        else if constexpr (c == '$')
        {
            transition <c, '('> (ctx);
        }

        else if constexpr (c == '#')
        {
            transition <c, '('> (ctx);
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
                transition <c, '{'> (ctx);

                
            } else if constexpr (c == '@')
            {
                reset (ctx);
                
            } else if constexpr (c == '#')
            {
//                template transition <STATE ("#{")> (ctx);
                transition <c, '{'> (ctx);


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
                transition <BEGIN> (ctx);
                
            } else
            {
                removeFromParent (ctx);
            }

        } else
        {
            ctx.result += ctx.potential;
            clear (ctx);
            transition <BEGIN> (ctx);
        }
    }
};
    

template <>
struct State2 <'#', '{'> : State2 <>
{
    virtual void _process (iter i, Context2& ctx){
        
        ctx.potential+= *i;
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                reset (ctx);
            }

        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
        }
    }
    
    virtual void addResultFromChild (string const& res, Context2& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasNoParent (Context2& ctx){
        ctx.potential.clear();
//            transition<STATE ("T(...){ done")>(ctx);
        transition <'#', '(', ')', '{', DONE> (ctx);

    }
    virtual void reset_hasParent (Context2& ctx){
        if (ctx.looping)
        {
//                TRANSITION ("begin");
            transition <BEGIN> (ctx);

        } else
        {
            removeFromParent (ctx);
        }
    }
    virtual string trans (){
        return "#{";
    }
 
};












template <>
struct State2 <'$', '(', '0'> : State2 <>
{
    void _process (iter i, Context2& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.firstint += *i;
        } else if (*i == ' ')
        {
            transition <'$', '(', '0', ' '> (ctx);
        } else
        {
            if (hasParent (ctx))
            {
                State2<>::addResultFromChild (ctx.potential, ctx);
                clear (ctx);
                transition <BEGIN> (ctx);
            } else
            {
                ctx.result += ctx.potential;
                clear (ctx);
                transition <BEGIN> (ctx);
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context2& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context2& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x";
    }
};

template <>
struct State2 <'$', '(', '0', ' '> : State2 <>
{
    void _process (iter i, Context2& ctx){
        
        
        if (*i == ' ')
        {
            ctx.potential += *i;
            
        } else if (*i == DECLPASTE)
        {
            addChildContext<'$'>(ctx).potential = DECLPASTE;
            
        }

        else
        {
            ctx.potential += *i;
            ctx.intvariable += *i;
            transition <'$', '(', '0', ' ', 'i'> (ctx);
        }
    }
    void addResultFromChild (string const& res, Context2& ctx){
        ctx.intvariable += res;
        transition <'$', '(', '0', ' ', 'i'> (ctx);
    }
    
    virtual void reset_hasNoParent (Context2& ctx){
        throw runtime_error ("f");
    }
    virtual void reset_hasParent (Context2& ctx){
        throw runtime_error ("f");
    }
    virtual string trans (){
        return "$(0 ";
    }
};

template <>
struct State2 <'$', '(', '0', ' ', 'i'> : State2 <>
{
    void _process (iter i, Context2& ctx){
        ctx.potential += *i;
        if (*i == ' ')
        {
            transition <'$', '(', '0', ' ', 'i', ' '> (ctx);

            
        } else
        {
            ctx.intvariable += *i;
        }
    }
};

template <>
struct State2 <'$', '(', '0', ' ', 'i', ' '> : State2 <>
{
    void _process (iter i, Context2& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
            transition <'$', '(', '0', ' ', 'i', ' ', '5'> (ctx);

        } else if (*i == ' ')
        {
            
        } else
        {
            if (hasParent (ctx))
            {
                addResultFromChild (ctx.potential, ctx);
                clear (ctx);
                transition <BEGIN> (ctx);

            } else
            {
                ctx.result += ctx.potential;
                clear (ctx);
                transition <BEGIN> (ctx);
            }
        }
    }
};

template <>
struct State2 <'$', '(', '0', ' ', 'i', ' ', '5'> : State2 <>
{
    void _process (iter i, Context2& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
        } else if (*i == ')')
        {
//            TRANSITION ("$(0 i 5)")
//            transition <State <STR ("T()"), STATE ("$(0 i 5)")>> (ctx);
        } else
        {
            if (hasParent (ctx))
            {
                addResultFromChild (ctx.potential, ctx);
                clear (ctx);
                transition <BEGIN> (ctx);
            } else
            {
                ctx.result += ctx.potential;
                clear (ctx);
                transition <BEGIN> (ctx);
            }
        }
    }
};
