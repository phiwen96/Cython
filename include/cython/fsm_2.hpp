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
//            Parent::template transition<State <STR ("{\nx"), self>>(ctx);
            Parent::template transition<'{', '\n', 'x', parent...>(ctx);
            
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
template <char c> struct State2 <c, '(', ')', '{'> : State2 <> {
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
template <char c> struct State2 <c, '(', ')'> : State2 <> {
    
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
            ctx.potential += ' ';
            
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
template <char c> struct State2 <c, '('> : State2 <> {
    
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
template <char c> struct State2 <c> : State2 <> {
    
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
template <> struct State2 <'$', '(', '0', ' ', 'i', ' ', '5'> : State2 <> {
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
template <> void State2<'@', '(', ')', '{'>::finish (Context2& ctx) {
    
    if (ctx.value.back () == '\n')
    {
        ctx.value.pop_back ();
    }
    
    declare (ctx.variable, ctx.value, ctx);
    clear (ctx);
    transition <'@', '(', ')', '{', DONE> (ctx);

//    TRANSITION ("T(...){ done")
}
