#pragma once
#include "fsm_2_base.hpp"


template <char... parent> struct State2 <'{', '\n', parent...> : State2 <parent...> {
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
            Parent::template transition<'{', '\n', 'x', '{', '\n', parent...>(ctx);
            
        } else
        {
            Parent::_process (i, ctx);
        }
    }
};
template <char... parent> struct State2 <'{', '\n', 'x', parent...> : State2 <parent...> {
    using Parent = State2 <parent...>;
    virtual void _process (iter i, Context2& ctx)
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
template <> void State2 <'$', '{'>::finish (Context2& ctx) {
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


template <> struct State2 <BEGIN> : State2 <> {
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
template <> struct State2 <DONE> : State2 <BEGIN> {};


template <char c> struct State2 <c, '(', ')', '{', DONE> : State2 <DONE> {
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


template <char c, char... r> struct State2 <c, '(', ')', '{', r...> : State2 <> {
    using self = State2 <c, '(', ')', '{', r...>;
    inline static constexpr bool loop = is_same_v <State2 <c, '(', ')', '{', r...>, State2 <c, '(', ')', '{', LOOP>>;
    
    
    virtual void _process (iter i, Context2& ctx)
    {
        if constexpr (not loop)
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
    virtual void addResultFromChild (string const& res, Context2& ctx)
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
};

template <char c, char... r> struct State2 <c, '(', ')', r...> : State2 <> {
    using self = State2 <c, '(', ')', r...>;
    virtual void _process (iter i, Context2& ctx)
    {
        ctx.potential += *i;
        
        if (*i == '{')
        {
            ctx.bracketStack.push ('{');
            transition <c, '(', ')', '{', r...> (ctx);

        } else if (*i == ' ')
        {
            ctx.potential += ' ';
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else
        {
            if (hasParent (ctx))
            {
                State2<>::addResultFromChild (ctx.potential, ctx);
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
template <char c> struct State2 <c, '('> : State2 <> {
    
    virtual void _process (iter i, Context2& ctx)
    {
        if (*i == ')')
        {
            ctx.potential += *i;
            transition <c, '(', ')'> (ctx);
            
        } else if (*i == '\n')
        {
            ctx.potential += '\n';
            
        } else if (*i == c)
        {
            addChildContext <c> (ctx);

        } else if (isnumber (*i))
        {
            ctx.potential += *i;
            ctx.firstint = *i;
            
            /**TODO generalize to @*/
            transition <'$', '(', '0'> (ctx);
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
template <char c> struct State2 <c> : State2 <> {
   
    void _process (iter i, Context2& ctx){
        
        ctx.potential += *i;
        
        if (c != '#' and *i == '(')
        {
            transition <c, '('> (ctx);
            
        } else if (c != '@' and *i == '{')
        {
            ctx.bracketStack.push ('{');
            transition <c, '{'> (ctx);
            
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
            State2<>::addResultFromChild (ctx.potential, ctx);
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


    



template <> struct State2 <'#', '{'> : State2 <> {
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












template <> struct State2 <'$', '(', '0'> : State2 <> {
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
template <> struct State2 <'$', '(', '0', ' '> : State2 <> {
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
template <> struct State2 <'$', '(', '0', ' ', 'i'> : State2 <> {
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
template <> struct State2 <'$', '(', '0', ' ', 'i', ' '> : State2 <> {
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
};
template <> struct State2 <'$', '(', '0', ' ', 'i', ' ', '5'> : State2 <> {
    void _process (iter i, Context2& ctx){
        ctx.potential += *i;
        if (isnumber (*i))
        {
            ctx.secondint += *i;
        } else if (*i == ')')
        {
            transition <'$', '(', ')', LOOP> (ctx);
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
};

template <> void State2<'$', '(', ')', '{'>::finish (Context2& ctx) {
    
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
    if (hasParent (ctx))
    {
        State2<>::addResultFromChild (ctx.value, ctx);
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
template <> void State2<'@', '(', ')', '{'>::finish (Context2& ctx) {
    
    if (ctx.value.back () == '\n')
    {
        ctx.value.pop_back ();
    }
    
    declare (ctx.variable, ctx.value, ctx);
    clear (ctx);
    transition <DONE> (ctx);

//    transition <'@', '(', ')', '{', DONE> (ctx);

//    TRANSITION ("T(...){ done")
}
template <> void State2<'$', '(', ')', '{', LOOP>::finish (Context2& ctx) {
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
        auto* childstate = new State2 <>;
        Context2* childctx = new Context2 {&ctx, ctx.declaredVariables, childstate};
        childstate -> transition <BEGIN> (*childctx);
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
        State2 <>::addResultFromChild (ctx.loop, ctx);
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
        
//                    removeFromParent (ctx);
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
