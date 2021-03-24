#pragma once
using namespace std;
#include <const_str/const_str.hpp>




template <unsigned int N>
constexpr char get_ch (char const (&s) [N], unsigned int i)
{
    return i >= N ? '\0' : s[i];
}


auto len = [] <size_t N> (char const (&s) [N]) constexpr {
    return N;
};

#define LEN(s) len(s) \
[] <size_t N> (char const (&s) [N]) constexpr { \
return N; \
}(s)

#define STRING_TO_CHARS_EXTRACT(z, n, data) \
        BOOST_PP_COMMA_IF(n) get_ch(data, n)

#define STRING_TO_CHARS(STRLEN, STR)  \
        BOOST_PP_REPEAT(STRLEN, STRING_TO_CHARS_EXTRACT, STR)


//#define STR(s) \
//        STRING_TO_CHARS(100, s)

//#define S(s) STR (s)//STR (BOOST_PP_STRINGIZE (s))

#define S(s) <STR (s)>

#define STATE(x) State <STR (x)>//S(x)



#define TRANSITION(x) transition <STATE (x)> (ctx);

#define BASE_STATE State <>

#define DECLPASTE '$'

#define DECL '@'

#define INDENTION 4



using iter = string::const_iterator;






struct Context;
template <class...>
struct State;

template <>
struct BASE_STATE
{
//    Context* context;
    using variable_iter = vector <pair <string, string>>::iterator;
    void process (iter i, Context& ctx);
    virtual void _process (iter i, Context& ctx){}
    bool hasParent (Context& ctx) const;
    string& variable (Context& ctx);
    string& value (Context& ctx);
    string& result (Context& ctx);
    string& potential (Context& ctx);
    void removeFromParent (Context& ctx);
    template <class T> void transition (Context& ctx);
    template <class T> Context& addChildContext (Context& ctx);
    State* parent (Context& ctx);
    void declare (string const& var, string const& val, Context& ctx);
    optional <string> declared (string const&, Context& ctx);
    virtual void addResultFromChild (string const& res, Context& ctx);
    void reset (Context& ctx);
    virtual void reset_hasNoParent (Context& ctx){}
    virtual void reset_hasParent (Context& ctx){}
    virtual string trans(){
//        throw runtime_error ("");
        return "";
    }
    string transi (Context& ctx);
    void chainChildren (iter i, Context& ctx);
    void clear (Context& ctx);
};

struct Context
{
    using variable_iter = vector <pair <string, string>>::iterator;
    
    
    Context* parent {nullptr};
    vector <pair <string, string>>& declaredVariables;
    State<>* state {nullptr};
    vector <Context*> children;
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
    
    void process (iter);
    
    
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

void BASE_STATE::clear (Context& ctx)
{
    ctx.variable.clear();
    ctx.value.clear();
    ctx.potential.clear();
    ctx.firstint.clear ();
    ctx.secondint.clear ();
    ctx.intvariable.clear ();
    ctx.loop.clear ();
}


void BASE_STATE::declare (string const& variable_name, string const& variable_value, Context& ctx) {
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
optional <string> BASE_STATE::declared (string const& p, Context& ctx) {
    throw runtime_error ("");
    for (auto d = ctx.declaredVariables.begin (); d != ctx.declaredVariables.end(); ++d) {
        if (d -> first == p) {
            return d->second;
        }
    }
    return {};
}

BASE_STATE* BASE_STATE::parent (Context& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr or ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    return ctx.parent -> state;
}

void BASE_STATE::reset (Context& ctx){
    if (not hasParent(ctx)) {
        reset_hasNoParent(ctx);
    } else {
        reset_hasParent(ctx);
    }
}

void Context::process(iter i) {
    state -> process (i, *this);
}

void BASE_STATE::addResultFromChild (string const& res, Context& ctx) {
#if defined (Debug)
    if (ctx.parent == nullptr || ctx.parent -> state == nullptr)
        throw runtime_error ("");
#endif
    ctx.parent -> state -> addResultFromChild (res, *ctx.parent);
}


void BASE_STATE::process (iter i, Context& ctx) {
    if (ctx.children.empty ()) {
//        ctx.curr_it = i;
        _process (i, ctx);
    } else
    {
        for (auto& o : ctx.children)
            o -> process (i);
    }
}

bool BASE_STATE::hasParent (Context& ctx) const {
    return ctx.parent != nullptr and ctx.parent->state != nullptr;
}

void BASE_STATE::removeFromParent (Context& ctx) {
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
Context& BASE_STATE::addChildContext (Context& ctx) {
//    cout << endl;
    State<>* childState = new Q;
    Context* childContext = new Context {&ctx, ctx.declaredVariables, childState};
    childContext -> max_indention = ctx.max_indention + INDENTION;
//    childState -> context = childContext;
    ctx.children.push_back (childContext);
    return *childContext;
#undef INDENTION
}

string& BASE_STATE::variable (Context& ctx) {
    return ctx.variable;
}
string& BASE_STATE::value (Context& ctx) {
    return ctx.value;
}
string& BASE_STATE::result (Context& ctx) {
    return ctx.result;
}
string& BASE_STATE::potential (Context& ctx) {
    return ctx.potential;
}

string BASE_STATE::transi (Context& ctx) {
    if (hasParent (ctx)){
        return ctx.parent->state->transi(*ctx.parent) + "::" + trans();
    } else
    {
        return trans();
    }
}
void BASE_STATE::chainChildren (iter i, Context& ctx) {
#if defined (Debug)
    if (ctx.children.empty ())
        throw runtime_error ("");
#endif
    for (auto& c : ctx.children)
    {
        c->process(i);
    }
}

template <>
struct STATE ("begin") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        if (*i == DECLPASTE)
        {
            potential (ctx) += *i;
            transition <State <STR ("T"), STATE ("$")>> (ctx);
//            TRANSITION ("$")
            
        } else if (*i == '#')
        {
            potential (ctx) += '#';
            transition <State <STR ("T"), STATE ("#")>> (ctx);
            
        } else if (*i == '@')
        {
            potential (ctx) += '@';
//            transition <State <STR ("T"), STATE ("@")>> (ctx);
//            TRANSITION ("@")
            transition <State <STR ("T"), STATE ("@")>> (ctx);
            
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (string {*i}, ctx);
                
            } else
            {
                result (ctx) += *i;
            }
        }
       

    }
    virtual void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "\"\"";
    }
};



template <class Parent>
struct State <STR ("{\n"), Parent> : Parent
{
    using self = State <STR ("{\n"), Parent>;
    virtual void _process (iter i, Context& ctx)
    {
        if (*i == '\n')
        {
            ctx.value += *i;
            ctx.indention.first = false;
            
        } else if (*i == ' ' and ctx.indention.first == false)
        {
            ++ctx.indention.second;
            Parent::template transition<State <STR ("{\nx"), self>>(ctx);
            
        } else
        {
            Parent::_process (i, ctx);
        }
    }
};

template <class Parent>
struct State <STR ("{\nx"), Parent> : Parent
{
    virtual void _process (iter i, Context& ctx)
    {
        if (*i == ' ')
        {
            ctx.potential += ' ';
            if (++ctx.indention.second == ctx.max_indention)
            {
                ctx.indention.second = 0;
                ctx.indention.first = true;
                Parent::template transition <Parent> (ctx);
            }
        } else
        {
            ctx.indention.second = 0;
            Parent::template transition <Parent> (ctx);
            Parent:: _process (i, ctx);
        }
    }
};

template <>
struct STATE ("#{") : BASE_STATE
{
    virtual void _process (iter i, Context& ctx){
        
        potential(ctx) += *i;
        
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
    
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasNoParent (Context& ctx){
        potential(ctx).clear();
        transition<STATE ("T(...){ done")>(ctx);
    }
    virtual void reset_hasParent (Context& ctx){
        if (ctx.looping)
        {
            TRANSITION ("begin");
        } else
        {
            removeFromParent (ctx);
        }
    }
    virtual string trans (){
        return "#{";
    }
 
};

















template <class T>
struct State <STR ("T(...){"), T> : T
{
    inline static constexpr bool parent_is_loop_type = is_same_v <T, STATE ("$(0 i 5){")>;
    using self = State <STR ("T(...){"), T>;
    virtual void _process (iter i, Context& ctx)
    {
        if constexpr (not parent_is_loop_type)
        {
            if (*i == DECLPASTE)
            {
                T::template addChildContext<State <STR ("T"), STATE ("$")>>(ctx).potential = *i;
                return;
                
            } else if (*i == '@')
            {
                T::template addChildContext<State <STR ("T"), STATE ("@")>>(ctx).potential = *i;
                return;
                
            } else if (*i == '#')
            {
                T::template addChildContext<State <STR ("T"), STATE ("#")>>(ctx).potential = *i;
                return;
                
            }
        }
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                T::finish (ctx);
                
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
            T::template transition <State <STR ("{\n"), self>> (ctx);
        } else
        {
            ctx.value += *i;
            ctx.potential += *i;
        }
    }
    virtual void addResultFromChild (string const& res, Context& ctx)
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
};

template <>
struct STATE ("${") : BASE_STATE {
    
    void finish (Context& ctx) {
            auto declared = ctx.findVariable (ctx.value);
            if (declared)
            {
                if (hasParent (ctx))
                {
                    BASE_STATE::addResultFromChild (declared.value()->second, ctx);
                    clear (ctx);
                    
                    if (ctx.looping)
                    {
                        TRANSITION ("begin")
                        
                    } else
                    {
                        removeFromParent (ctx);
                    }
                } else
                {
                    result(ctx) += declared.value ()->second;
                    clear (ctx);
                    TRANSITION ("done")
                }
                
            } else
           {
               string warning = "variable \"" + value (ctx) + "\" pasted but it has not yet been declared!";
               throw runtime_error (warning);
           }
        
    }
};

template <>
struct STATE ("$(){") : BASE_STATE {
    void finish (Context& ctx) {
        if (auto found = ctx.findVariable (ctx.variable); found)
        {
            found.value()->second = ctx.value;
            
        } else
        {
            ctx.declaredVariables.emplace_back(variable(ctx), value(ctx));
        }
        
        if (ctx.value.back () == '\n')
        {
            ctx.value.pop_back ();
        }
        if (hasParent(ctx))
        {
            BASE_STATE::addResultFromChild (value(ctx), ctx);
            clear (ctx);
            
            if (ctx.looping)
            {
                TRANSITION ("begin")
                
            } else
            {
                removeFromParent(ctx);
            }

        } else
        {
            result (ctx) += value (ctx);
            clear (ctx);
            TRANSITION ("done")
        }
    }
};

template <>
struct STATE ("@(){") : BASE_STATE {
    void finish (Context& ctx) {
        if (ctx.value.back () == '\n')
        {
            ctx.value.pop_back ();
        }
        
        declare (variable (ctx), value (ctx), ctx);
        clear (ctx);
        TRANSITION ("T(...){ done")
    }
};

template <>
struct STATE ("$(0 i 5){") : BASE_STATE {
    void finish (Context& ctx) {
        int i = stoi (ctx.firstint);
        int end = stoi (ctx.secondint);
        
        for (; i < end; ++i)
        {
            declare (ctx.intvariable, to_string (i), ctx);
//                    cout << ctx.intvariable << to_string (i) << endl;
//                    cout << "bajs::" << ctx.value << endl;
//                    addChildContext <STATE ("begin")> (ctx);
//                    string temp = ctx.value;
//                    ctx.value.clear ();
            auto* childstate = new BASE_STATE;
            Context* childctx = new Context {&ctx, ctx.declaredVariables, childstate};
            childstate -> transition <STATE ("begin")> (*childctx);
//                    ctx.children.push_back (childctx);
//                addChildContext <STATE ("begin")> (ctx);
            childctx -> looping = true;
            for (iter j = ctx.value.begin (); j < ctx.value.end (); ++j)
            {
                
                childctx -> process (j);
            
//                        chainChildren (j, ctx);
            }
            childctx -> looping = false;
            
//                    cout << "kuk" << endl;
            delete childstate;
            delete childctx;
        }
        auto declared = ctx.findVariable (ctx.intvariable);
        if (declared)
        {
            ctx.declaredVariables.erase (declared.value ());
        }
        
        if (ctx.loop.back () == '\n')
        {
            cout << "hi" << endl;
            /**
             fix so that:
             
                 $ (0 i 2)
                 {
                     hej
                 }
                 kuk
             
             becomes:
             
                hej
                kuk
             
             and not:
                hej
                
                kuk
             
             
             
             */
            ctx.loop.pop_back ();
        }
        
        
        
        if (hasParent (ctx))
        {
            BASE_STATE::addResultFromChild (ctx.loop, ctx);
            potential (ctx).clear ();
            ctx.variable.clear ();
            ctx.value.clear ();
            ctx.firstint.clear ();
            ctx.secondint.clear ();
            ctx.intvariable.clear ();
            ctx.loop.clear ();
            ctx.bracketStack = stack <char> {};
            
            if (ctx.looping)
            {
                TRANSITION ("begin")
                
            } else
            {
                removeFromParent (ctx);
            }
            
//                    removeFromParent (ctx);
        } else
        {
            result (ctx) += ctx.loop;
            potential (ctx).clear ();
            ctx.variable.clear ();
            ctx.value.clear ();
            ctx.firstint.clear ();
            ctx.secondint.clear ();
            ctx.intvariable.clear ();
            ctx.loop.clear ();
            ctx.bracketStack = stack <char> {};
            TRANSITION ("begin");
        }
    }
};













template <class T>
struct State <STR ("T()"), T> : T
{
    void _process (iter i, Context& ctx)
    {
        ctx.potential += *i;
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            T::finish (ctx);
//            T::template transition <State <STR ("T(...){"), STATE ("$(0 i 5){")>> (ctx);
//            TRANSITION ("$(0 i 5){");
        } else if (*i == ' ')
        {

        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else
        {
            if (T::hasParent (ctx))
            {
                T::addResultFromChild (ctx.potential, ctx);
                T::clear (ctx);
                T::template transition <STATE ("begin")> (ctx);
//                TRANSITION ("begin");
//                removeFromParent (ctx);
            } else
            {
                ctx.result += ctx.potential;
                T::clear (ctx);
                T::template transition <STATE ("begin")> (ctx);
            }
        }
    }
};

template <>
struct STATE ("$()") : BASE_STATE
{
    void finish (Context& ctx)
    {
        transition <State <STR ("T(...){"), STATE ("$(){")>> (ctx);
    }
};

template <>
struct STATE ("@()") : BASE_STATE
{
    void finish (Context& ctx)
    {
        transition <State <STR ("T(...){"), STATE ("@(){")>> (ctx);
    }
};

template <>
struct STATE ("$(0 i 5)") : BASE_STATE
{
    void finish (Context& ctx)
    {
        transition <State <STR ("T(...){"), STATE ("$(0 i 5){")>> (ctx);
    }
};















template <class T>
struct State <STR ("T("), T> : T
{
    inline static constexpr bool dollar = is_same_v <T, STATE ("$(")>;
    inline static constexpr bool alpha = is_same_v <T, STATE ("@(")>;
//    inline static constexpr bool hashtag = is_same_v <T, STATE ("#")>;
    
    virtual void _process (iter i, Context& ctx)
    {
        if (*i == ')')
        {
            ctx.potential += *i;
            
            if constexpr (dollar)
                T::template transition <State <STR ("T()"), STATE ("$()")>> (ctx);
            
            else if constexpr (alpha)
                T::template transition <State <STR ("T()"), STATE ("@()")>> (ctx);
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else if (*i == DECLPASTE)
        {
            T::template addChildContext <State <STR ("T"), STATE ("$")>> (ctx).potential = *i;

        } else if (*i == '@')
        {
            T::template addChildContext <State <STR ("T"), STATE ("@")>> (ctx).potential = *i;
            
        } else if (isnumber (*i))
        {
            ctx.potential += *i;
            ctx.firstint = *i;
            
            if constexpr (dollar)
            {
//                T::template transition <State <STR ("T()"), STATE ("$(0")>> (ctx);
                T::template transition <STATE ("$(0")> (ctx);
                
            } else if constexpr (alpha)
            {
//                T::template transition <State <STR ("T()"), STATE ("@(x")>> (ctx);
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

template <>
struct STATE ("$(") : BASE_STATE {};

template <>
struct STATE ("@(") : BASE_STATE {};














template <>
struct STATE ("$(0") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        potential (ctx) += *i;
        if (isnumber (*i))
        {
            ctx.firstint += *i;
        } else if (*i == ' ')
        {
            TRANSITION ("$(0 ");
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                clear (ctx);
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                clear (ctx);
                TRANSITION ("begin");
            }
        }
    }
    void addResultFromChild (string const& res){
        throw runtime_error ("oops");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x";
    }
};

template <>
struct STATE ("$(0 ") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        
        
        if (*i == ' ')
        {
            ctx.potential += *i;
            
        } else if (*i == DECLPASTE)
        {
            addChildContext<State <STR ("T"), STATE ("$")>>(ctx).potential = DECLPASTE;
            
        }

        else
        {
            ctx.potential += *i;
            ctx.intvariable += *i;
            TRANSITION ("$(0 i");
        }
    }
    void addResultFromChild (string const& res, Context& ctx){
        ctx.intvariable += res;
        TRANSITION ("$(0 i");
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("f");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("f");
    }
    virtual string trans (){
        return "$(0 ";
    }
};

template <>
struct STATE ("$(0 i") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (*i == ' ')
        {
            TRANSITION ("$(0 i ");
            
        } else
        {
            ctx.intvariable += *i;
        }
    }
};

template <>
struct STATE ("$(0 i ") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
            TRANSITION ("$(0 i 5")
        } else if (*i == ' ')
        {
            
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                clear (ctx);
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                clear (ctx);
                TRANSITION ("begin");
            }
        }
    }
};

template <>
struct STATE ("$(0 i 5") : BASE_STATE
{
    void _process (iter i, Context& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
        } else if (*i == ')')
        {
//            TRANSITION ("$(0 i 5)")
            transition <State <STR ("T()"), STATE ("$(0 i 5)")>> (ctx);
        } else
        {
            if (hasParent (ctx))
            {
                BASE_STATE::addResultFromChild (potential (ctx), ctx);
                clear (ctx);
                TRANSITION ("begin");
            } else
            {
                result (ctx) += potential (ctx);
                clear (ctx);
                TRANSITION ("begin");
            }
        }
    }
};

template <class T>
struct State <STR ("T"), T> : T
{
    inline static constexpr bool dollar = is_same_v <T, STATE ("$")>;
    inline static constexpr bool alpha = is_same_v <T, STATE ("@")>;
    inline static constexpr bool hashtag = is_same_v <T, STATE ("#")>;
    
    void change_state (Context& ctx)
    {
        if constexpr (alpha)
            T::template transition <State <STR ("T("), STATE ("@(")>> (ctx);
        
        else if constexpr (dollar)
            T::template transition <State <STR ("T("), STATE ("$(")>> (ctx);

        else if constexpr (hashtag)
            T::template transition <STATE ("#{")> (ctx);
    }
   
    void _process (iter i, Context& ctx){
        
        ctx.potential += *i;
        
        if (*i == '(')
        {
            if constexpr (alpha)
            {
                change_state (ctx);
                
            } else if constexpr (dollar)
            {
                change_state (ctx);
                
            } else if constexpr (hashtag)
            {
                reset (ctx);
            }
           
        } else if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            
            if constexpr (dollar)
            {
                T::template transition <State <STR ("T(...){"), STATE ("${")>> (ctx);
                
            } else if constexpr (alpha)
            {
                reset (ctx);
                
            } else if constexpr (hashtag)
            {
                T::template transition <STATE ("#{")> (ctx);
            }
            
        } else if (*i == ' ')
        {
            if constexpr (not is_same_v <T, STATE ("#")>)
            ctx.potential += ' ';
            
        } else if (*i == '\n')
        {
            if constexpr (not is_same_v <T, STATE ("#")>)
            ctx.potential += '\n';
            
        } else
        {
            reset (ctx);
        }
    }
    
    void reset (Context& ctx)
    {
        if (T::hasParent (ctx))
        {
            T::addResultFromChild (ctx.potential, ctx);
            T::clear (ctx);
            
            if (ctx.looping)
            {
                T::template transition <STATE ("begin")> (ctx);
                
            } else
            {
                T::removeFromParent (ctx);
            }

        } else
        {
            ctx.result += ctx.potential;
            T::clear (ctx);
            T::template transition <STATE ("begin")> (ctx);
        }
    }
};

template <>
struct STATE ("$") : BASE_STATE {};

template <>
struct STATE ("@") : BASE_STATE {};

template <>
struct STATE ("#") : BASE_STATE {};

template <>
struct STATE ("done") : STATE ("begin")
{
    virtual void _process (iter i, Context& ctx){
        STATE ("begin")::_process (i, ctx);
    }
    
    virtual void reset_hasNoParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context& ctx){
        throw runtime_error ("");
    }
    virtual void addResultFromChild (string const& res, Context& ctx){
        throw runtime_error ("");
    }
};

template <>
struct State <STR ("T(...){ done")> : STATE ("done")
{
    virtual void _process (iter i, Context& ctx){
        if (*i == '\n')
        {
            ctx.potential += '\n';
            TRANSITION ("done")
        } else
        {
            TRANSITION ("done")
            STATE ("done")::_process (i, ctx);
        }
    }
};

template <class T>
void BASE_STATE::transition (Context& ctx) {
    
//    cout << transi(ctx) << " -> ";

    
    ctx.state = new T;
    
    
//    cout << ctx.state->transi(ctx) << endl;
//    cout << ctx.result << endl;
}


/**
 
 ${fornamn${fornamn}}
 

try
   {
       int i = std::stoi(s);
       std::cout << i << '\n';
   }
   catch (std::invalid_argument const &e)
   {
       std::cout << "Bad input: std::invalid_argument thrown" << '\n';
   }
   catch (std::out_of_range const &e)
   {
       std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
   }


${hej}
$(fornamn){Philip}
${fornamn}
$(kiss){bajs}
$(snopp){${fornamn}}
$(namn){${fornamn}$(efternamn){Wenkel}}
*/
