#pragma once
#include "fsm_2_base.hpp"


template <char... parent> struct State2 <'{', '\n', parent...> : State2 <parent...> {
    using self = State2 <parent..., '\n'>;
    using Parent = State2 <parent...>;
    virtual void _process (iter i, Context2& ctx) override
    {
        if (*i == '\n')
        {
            ctx.value += *i;
            ctx.indention.first = false;
            
        } else if (*i == ' ' and ctx.indention.first == false)
        {
            ++ctx.indention.second;
            Parent::template transition<'{', '\n', 'x', '{', '\n', parent...>(ctx);
            
        } else
        {
            Parent::_process (i, ctx);
        }
    }
};
template <char... parent> struct State2 <'{', '\n', 'x', parent...> : State2 <parent...> {
    using Parent = State2 <parent...>;
    virtual void _process (iter i, Context2& ctx) override
    {
        if (*i == ' ')
        {
            ctx.potential += ' ';
            
            /**
             check if current ++indentions == 4, if so then reset indention for next
             */
            if (++ctx.indention.second == ctx.max_indention)
            {
                ctx.indention.second = 0;
                ctx.indention.first = true;
                
                // go back to {\n so that doesnt $(){ skip newlines
                Parent::template transition <parent...> (ctx);
            }
            
        } else
        {
            ctx.indention.second = 0;
            ctx.indention.first = false;
            Parent::template transition <parent...> (ctx);
            Parent:: _process (i, ctx);
        }
    }
};



template <char c> struct State2 <c, '{'> : State2 <> {
    inline static constexpr bool parent_is_loop_type = false;//is_same_v <T, STATE ("$(0 i 5){")>;
//    using self = State2 <c, '(', ')', '{'>;
    
    virtual void _process (iter i, Context2& ctx) override
    {
//        cout << "yo" << endl;
        if constexpr (not parent_is_loop_type)
        {
            auto fun = [i, this, &ctx] {
                this -> addChildContext <c> (ctx).potential = *i;
            };
            
            if (is_key (i, fun))
                return;
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
    virtual void addResultFromChild (string&& res, Context2& ctx) override
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
    virtual ~State2 () override {
        
    }
};
template <> void State2 <DECLPASTE, '{'>::finish (Context2& ctx) {

    auto declared = ctx.findVariable (ctx.value);
    
    if (declared)
    {
        IF_CHILD
        {
//            cout << "lol" << endl;
//            cout << ":" << declared.value () -> second << endl;
            State2 <>::addResultFromChild ((string&&) declared.value()->second, ctx);
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


template <> struct State2 <BEGIN> : State2 <> {
#define STAND_BY ctx.result += *i;
    
    template <char c>
    void go_to_next_state (iter i, Context2& ctx) {
        ctx.potential += *i;
        transition <c> (ctx);
    }
    
    virtual void _process (iter i, Context2& ctx)  override{
        
        IF_KEY (go_to_next_state)
        
        else IF_CHILD
        {
            PUSH_PARENT (string {*i})
        }
        
        else
        {
            STAND_BY
        }
        
       

    }
    virtual ~State2 () override {
        
    }
#undef STAND_BY
};
template <> struct State2 <DONE> : State2 <BEGIN> {
    virtual ~State2 () override {
        
    }
};


template <char c> struct State2 <c, '(', ')', '{', DONE> : State2 <DONE> {
    virtual void _process (iter i, Context2& ctx)  override {
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
    virtual ~State2 () override {
        
    }
};


template <char c, char... r> struct State2 <c, '(', ')', '{', r...> : State2 <> {
    using self = State2 <c, '(', ')', '{', r...>;
    inline static constexpr bool loop = is_same_v <State2 <c, '(', ')', '{', r...>, State2 <c, '(', ')', '{', LOOP>>;
    
    
    virtual void _process (iter i, Context2& ctx) override
    {
        if constexpr (not loop)
        {
            if (*i == DECLPASTE)
            {
//                cout << "hi" << endl;
                addChildContext <DECLPASTE> (ctx).potential = *i;
                return;
                
            } else if (*i == DECLARE)
            {
                addChildContext <DECLARE> (ctx).potential = *i;
                return;
                
            } else if (*i == COMMENT)
            {
                addChildContext <COMMENT> (ctx).potential = *i;
                return;
                
            }
        }
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            if (ctx.bracketStack.empty ())
            {
                finish (ctx);
                
                if constexpr (c == '@')
                {
                    using next_transition = get_next_transition <c, SUCCESS>;
                    
                    ctx.transition_to <next_transition> ();
                }
                
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
            
        }
        else if (*i == '\n')
        {
//            cout << "hi" << endl;
            ctx.potential += '\n';
//            transition <State <STR ("{\n"), self>> (ctx);
            transition <'{', '\n', c, '(', ')', '{', r...> (ctx);

        }
        else
        {
            ctx.value += *i;
            ctx.potential += *i;
        }
    }
    virtual void addResultFromChild (string&& res, Context2& ctx) override
    {
        if constexpr (loop)
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
    virtual ~State2 () override {
        
    }
};

template <char...>
struct Finish;

template <>
struct Finish <DECLARE>
{
    void finish (Context2& ctx)
    {
//        if (ctx.value.back () == '\n')
//        {
//            ctx.value.pop_back ();
//        }
//
//        declare (ctx.variable, ctx.value, ctx);
//        clear (ctx);
    }
};

template <char c, char... r> struct State2 <c, '(', ')', r...> : State2 <> {
    using self = State2 <c, '(', ')', r...>;
#define IF_SUCCESS if (*i == '{')
#define EVOLVE \
    ctx.bracketStack.push ('{'); \
    transition <c, '(', ')', '{', r...> (ctx);
    
#define REGRESS_AS_MAIN \
    ctx.result += ctx.potential; \
    clear (ctx); \
    transition <BEGIN> (ctx);
    
#define REGRESS_AS_CHILD \
    State2<>::addResultFromChild ((string&&) ctx.potential, ctx); \
    clear (ctx); \
    transition <BEGIN> (ctx);
    
#define ON_FAILURE
    
#define IF_REFORMATTING if (*i == ' ' or *i == '\n')
    
    virtual void _process (iter i, Context2& ctx) override
    {
        ctx.potential += *i;
        
        IF_SUCCESS
        {
            EVOLVE

        } else IF_REFORMATTING
        {
//            ctx.potential += '\n';
            
        } else ON_FAILURE
        {
            IF_CHILD
            {
                REGRESS_AS_CHILD
//                TRANSITION ("begin");
//                removeFromParent (ctx);
            } else
            {
                REGRESS_AS_MAIN
            }
        }
    }
    
    void finish (Context2& ctx);
    virtual ~State2 () override {
        
    }
    
};
template <char c> struct State2 <c, '('> : State2 <> {
#define IF_SUCCESS if (*i == ')')
#define EVOLVE transition <c, '(', ')'> (ctx);
#define IF_PREGNANT if (*i == DECLPASTE || *i == DECLARE)
#define ADD_CHILD \
    if (*i == DECLPASTE) \
        addChildContext <DECLPASTE> (ctx); \
    else if (*i == DECLARE) \
        addChildContext <DECLARE> (ctx);//.potential += *i;
//#define IF_STAND_BY_0

    
    virtual void _process (iter i, Context2& ctx) override
    {
        
        ADD_POTENTIAL
        
        IF_SUCCESS
        {
            EVOLVE
            
        } else IF_PREGNANT
        {
            ADD_CHILD
            
        } else if (isdigit (*i))
        {
//            ctx.potential += *i;
            ctx.firstint = *i;
            
            /**TODO generalize to @*/
            transition <DECLPASTE, '(', '0'> (ctx);
        } else
        {
            ctx.variable += *i;
//            ctx.potential += *i;
        }
    }
    
    virtual void addResultFromChild (string&& res, Context2& ctx) override {
        ctx.variable += (string&&) res;
    }
    virtual ~State2 () override {
        
    }
};
template <char c> struct State2 <c> : State2 <> {
   
    void _process (iter i, Context2& ctx) override{
        
        ctx.potential += *i;
        
        if (c != COMMENT and *i == '(')
        {
            transition <c, '('> (ctx);
            
        } else if (c != DECLARE and *i == '{')
        {
            ctx.bracketStack.push ('{');
            transition <c, '{'> (ctx);
            
        } else if (*i == ' ')
        {
            if constexpr (c != COMMENT)
                ctx.potential += ' ';
            
        } else if (*i == '\n')
        {
            if constexpr (c != COMMENT)
                ctx.potential += '\n';
            
        } else
        {
            reset (ctx);
        }
    }
    
    void reset (Context2& ctx)
    {
        IF_CHILD
        {
            PUSH_PARENT ((string&&) ctx.potential)
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
    virtual ~State2 () override {
        
    }
};


    



template <> struct State2 <COMMENT, '{'> : State2 <> {
    
#define EVOLVE_AS_MAIN \
    ctx.potential.clear(); \
    transition <COMMENT, '(', ')', '{', DONE> (ctx);
    
#define EVOLVE_AS_CHILD \
    if (ctx.looping) \
        transition <BEGIN> (ctx); \
    else \
        removeFromParent (ctx);

#define IF_SUCCESS \
    if (ctx.bracketStack.empty ())
    
#define STAND_BY ctx.bracketStack.push ('{');
    
    virtual void _process (iter i, Context2& ctx) override {
        
        ADD_POTENTIAL
        
        if (*i == '}')
        {
            ctx.bracketStack.pop ();
            
            IF_SUCCESS
            {
//                reset (ctx);
                IF_CHILD
                {
                    EVOLVE_AS_CHILD
                    
                } else
                {
                    EVOLVE_AS_MAIN
                }
            }

        } else if (*i == '{')
        {
            STAND_BY
        }
    }
    
    virtual void addResultFromChild (string&& res, Context2& ctx) override{
        throw runtime_error ("");
    }

    virtual ~State2 () override {
        
    }
};







// WHY
/**
 WHY
 
 a                          a
 @ (k) {K}
 b                          a
 
 a                         a
 # {K}
 a                         a
 
 a                         a
 $ (k) {K}             K
 a                         a
 
 */
template <> struct State2 <DONE, NO_PASTE> : State2 <DONE>
{
    using next = State2 <DONE>;
    
    virtual void _process (iter i, Context2& ctx) override {
        
        if (*i == '\n')
        {
            ADD_POTENTIAL
            
            ctx.transition_to <next> ();
            
        } else
        {
            transition <DONE> (ctx);
            State2 <DONE>::_process (i, ctx);
        }
    }
    virtual ~State2 () override {
        
    }
};


template <> struct State2 <DECLPASTE, '(', '0'> : State2 <> {
#define EVOLVE transition <DECLPASTE, '(', '0', ' '> (ctx);
    
    void _process (iter i, Context2& ctx) override{
        
        ADD_POTENTIAL
        
        if (isdigit (*i))
        {
            ctx.firstint += *i;
            
        } else if (*i == ' ')
        {
            EVOLVE
            
        } else
        {
            IF_CHILD
            {
                PUSH_PARENT ((string&&) ctx.potential)
                
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

    
    virtual void reset_hasNoParent (Context2& ctx) override{
        throw runtime_error ("");
    }
    virtual void reset_hasParent (Context2& ctx) override{
        throw runtime_error ("");
    }
    virtual string trans (){
        return "$(x";
    }
    virtual ~State2 () override {
        
    }
};
template <> struct State2 <DECLPASTE, '(', '0', ' '> : State2 <> {
#define GIVE_BIRTH addChildContext<DECLPASTE>(ctx);//.potential = DECLPASTE;
#define EVOLVE \
    ctx.intvariable += *i; \
    transition <DECLPASTE, '(', '0', ' ', 'i'> (ctx);
    
    void _process (iter i, Context2& ctx) override{

        ADD_POTENTIAL
        
        if (*i == DECLPASTE)
        {
            GIVE_BIRTH
        }

        else
        {
            EVOLVE
        }
    }
    void addResultFromChild (string&& res, Context2& ctx) override{
        ctx.intvariable += res;
        transition <DECLPASTE, '(', '0', ' ', 'i'> (ctx);
    }
    
    virtual void reset_hasNoParent (Context2& ctx) override{
        throw runtime_error ("f");
    }
    virtual void reset_hasParent (Context2& ctx) override{
        throw runtime_error ("f");
    }
    virtual string trans (){
        return "$(0 ";
    }
    virtual ~State2 () override {
        
    }
};
template <> struct State2 <DECLPASTE, '(', '0', ' ', 'i'> : State2 <> {
    void _process (iter i, Context2& ctx) override{
        ctx.potential += *i;
        if (*i == ' ')
        {
            transition <DECLPASTE, '(', '0', ' ', 'i', ' '> (ctx);

            
        } else
        {
            ctx.intvariable += *i;
        }
    }
    virtual ~State2 () override {
        
    }
};
template <> struct State2 <DECLPASTE, '(', '0', ' ', 'i', ' '> : State2 <> {
    void _process (iter i, Context2& ctx) override{
        ctx.potential += *i;
        if (isdigit (*i))
        {
            ctx.secondint += *i;
            transition <DECLPASTE, '(', '0', ' ', 'i', ' ', '5'> (ctx);

        } else if (*i == ' ')
        {
            
        } else
        {
            if (hasParent (ctx))
            {
                State2<>::addResultFromChild ((string&&) ctx.potential, ctx);
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
    virtual ~State2 () override {
        
    }
};
template <> struct State2 <DECLPASTE, '(', '0', ' ', 'i', ' ', '5'> : State2 <> {
#define EVOLVE transition <DECLPASTE, '(', ')', LOOP> (ctx);
    
#define REGRESS_AS_MAIN \
    ctx.result += ctx.potential; \
    clear (ctx); \
    transition <BEGIN> (ctx);
    
#define REGRESS_AS_CHILD \
    State2<>::addResultFromChild ((string&&) ctx.potential, ctx); \
    clear (ctx); \
    transition <BEGIN> (ctx);

    
    
    void _process (iter i, Context2& ctx) override {
        
        ADD_POTENTIAL
        
        if (isdigit (*i))
        {
            ctx.secondint += *i;
            
        } else if (*i == ')')
        {
            EVOLVE
            
        } else
        {
            IF_CHILD
            {
                REGRESS_AS_CHILD
                
            } else
            {
                REGRESS_AS_MAIN
            }
        }
    }
    virtual ~State2 () override {
        
    }
};

template <> void State2<DECLPASTE, '(', ')', '{'>::finish (Context2& ctx) {
    
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
    
    IF_CHILD
    {
        PUSH_PARENT ((string&&) ctx.value)
        
        
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
template <> void State2<DECLARE, '(', ')', '{'>::finish (Context2& ctx) {
    
    if (ctx.value.back () == '\n')
    {
        ctx.value.pop_back ();
    }
    
    ctx.declare_variable ((string&&) ctx.variable, (string&&) ctx.value);
//    declare (ctx.variable, ctx.value, ctx);

    clear (ctx);
//    transition <DONE, NO_PASTE> (ctx);
}
template <> void State2<DECLPASTE, '(', ')', '{', LOOP>::finish (Context2& ctx) {
    int i = stoi (ctx.firstint);
    int end = stoi (ctx.secondint);
    
    for (; i < end; ++i)
    {
        ctx.declare_variable (ctx.intvariable, to_string (i));
        auto* childstate = new State2 <>;
        Context2* childctx = new Context2 {&ctx, ctx.declaredVariables, childstate};
        childstate -> transition <BEGIN> (*childctx);
        childctx -> looping = true;
        for (iter j = ctx.value.begin (); j < ctx.value.end (); ++j)
        {
            childctx -> process (j);
        }
        childctx -> looping = false;

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
        State2 <>::addResultFromChild ((string&&) ctx.loop, ctx);
        ctx.potential.clear ();
        ctx.variable.clear ();
        ctx.value.clear ();
        ctx.firstint.clear ();
        ctx.secondint.clear ();
        ctx.intvariable.clear ();
        ctx.loop.clear ();
        ctx.bracketStack = stack <char> {};
        
        if (ctx.looping)
        {
            transition <BEGIN> (ctx);

        } else
        {
            removeFromParent (ctx);
        }
        
    } else
    {
        ctx.result += ctx.loop;
        ctx.potential.clear ();
        ctx.variable.clear ();
        ctx.value.clear ();
        ctx.firstint.clear ();
        ctx.secondint.clear ();
        ctx.intvariable.clear ();
        ctx.loop.clear ();
        ctx.bracketStack = stack <char> {};
        transition <BEGIN> (ctx);
    }
}




template <>
struct _get_next_transition <DECLARE, SUCCESS>
{
    using type = decltype (State2 <DONE, NO_PASTE> {});
};
