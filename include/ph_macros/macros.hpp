#pragma once
using namespace std;

template <unsigned int N>
constexpr char get_ch (char const (&s) [N], unsigned int i)
{
    return i >= N ? '\0' : s[i];
}

namespace {
template <size_t N, size_t M>
consteval bool same_strings (char const(&lhs)[N], char const(&rhs)[M])
{
    if (N != M)
        return false;
    
    for (int i = 0; i < N; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    
    return true;
}

template <size_t N>
consteval char charat (char const(&s)[N], int i) {
    return s[i];
}

template <size_t N>
consteval size_t string_size (char const(&)[N])
{
    return N;
}



#define PH_$empty()



/*
 delay the evaluation of a macro
 
 @example
    foo (x, y) x,  y
    bar () 1, 2
        
        foo (bar ())            ERROR
        defer (foo) (bar ())    foo (1, 2)
 DEFER(A)() expands to A()
 */
#define PH_$defer(...) __VA_ARGS__ PH_$empty ()

#define INC(x) PRIMITIVE_CAT(INC_, x)
#define INC_0 1
#define INC_1 2
#define INC_2 3
#define INC_3 4
#define INC_4 5
#define INC_5 6
#define INC_6 7
#define INC_7 8
#define INC_8 9
#define INC_9 9

#define DEC(x) PRIMITIVE_CAT(DEC_, x)
#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8

#define PROBE(x) x, 1,

#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0


#define BOOL(x) COMPL(NOT(x))

#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)


#define IS_PAREN(x) CHECK(IS_PAREN_PROBE x)
#define IS_PAREN_PROBE(...) PROBE(~)




#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__

#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define DEFER(...) __VA_ARGS__   EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EMPTY()
#define CAT(a, ...) PRIMITIVE_CAT (a, __VA_ARGS__)
    #define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define IF(cond) EVAL (DEFER (CAT) (IF_, BOOL (cond)))
    #define IF_0(...)
    #define IF_1(...) __VA_ARGS__
#define IF_ELSE(cond) EVAL (DEFER (PRIMITIVE_CAT) (IF_ELSE_, BOOL(cond)))
    #define IF_ELSE_0(...) EXPAND
    #define IF_ELSE_1(...) __VA_ARGS__ EAT


#define WHILE(pred, op, ...) \
    IF(pred(__VA_ARGS__)) \
    ( \
        OBSTRUCT(WHILE_INDIRECT) () \
        ( \
            pred, op, op(__VA_ARGS__) \
        ), \
        __VA_ARGS__ \
    )
#define WHILE_INDIRECT() WHILE




//#define IF_TOKEN_IS_STRING(token)
//#define IF_ELSE_TOKEN_IS_STRING(token) IF_ELSE (IF_TOKEN_IS_STRING (token, string) (IF_1_ELSE) (IF_0_ELSE)) (1) (0)

#define PH_MACRO_$expand(...) __VA_ARGS__

#define PH_MACRO_$string_size(x) string_size (x)

#define PH_MACRO(...) __VA_ARGS__


#define TO_UPPERCASE_COUNTED(Str,Cnt)
#define TO_UPPER(Str) TO_UPPERCASE_COUNTED(Str,__COUNT__) {( \
   static char buf_##Cnt[sizeof(Str)+4]; \
   char *str_##Cnt = Str; \
   int ix_##Cnt = 0; \
   for (; *str_##Cnt; str_##Cnt++, ix_##Cnt++) \
     if (ix_##Cnt < sizeof(buf_##Cnt)-1) \
         buf_##Cnt[ix_##Cnt] = toupper(*str_##Cnt); \
   buf_##Cnt; )}



//#define TO_UPPER(x)

#define CHAR_AT_TOKEN (token, i)



#define PH_MACRO_$token_is_string(x, y) __builtin_choose_expr (same_strings (BOOST_PP_STRINGIZE (x), y), 1, 0)
#define PH_MACRO_$if_token_is_string(token, string, true, false) __builtin_choose_expr (PH_MACRO_$token_is_string (token, string), true, false)
#define PH_MACRO_$has(x) \
    [] (auto a) constexpr { \
        return requires (){a.x;}; \
    }

}
namespace {
consteval auto my_function_name (char const* s = __builtin_FUNCTION ()) {
    return s;
}
#define MY_FUNCTION_NAME my_function_name ()
}






/**
 BOOST_PP_SEQ_ENUM
 BOOST_PP_SEQ_REST_N
 BOOST_PP_REPEAT_FROM_TO
 */



#define CALL_MACRO(_,__,SEQ) Macro(Foo<BOOST_PP_SEQ_ENUM(SEQ)>)

#define GENERATE_MACRO_INVOCATIONS(SEQS) BOOST_PP_SEQ_FOR_EACH(CALL_MACRO,_,SEQS)

#define GENERATE_DESCENDING_SEQUENCES(_,INDEX,DATA) (BOOST_PP_SEQ_REST_N(INDEX,DATA))

#define BAR_IMPL(SEQ) GENERATE_MACRO_INVOCATIONS(BOOST_PP_REPEAT_FROM_TO(0,BOOST_PP_SEQ_SIZE(SEQ),GENERATE_DESCENDING_SEQUENCES, SEQ))


#define Bar(...) BAR_IMPL(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

/**
 Bar(float, int, string, vector<int>)

 Macro(Foo<float, int, string, vector<int>>)
 Macro(Foo<int, string, vector<int>>)
 Macro(Foo<string, vector<int>>)
 Macro(Foo<vector<int>>)
 
 
 
 
 
 
 #define TEXT(z, n, text) BOOST_PP_COMMA_IF(n) text

 #define TEMPLATE(z, n, _) \
    BOOST_PP_COMMA_IF(n) \
    template< \
       BOOST_PP_REPEAT_ ## z( \
          BOOST_PP_INC(n), \
          TEXT, class \
       ) \
    > kuk T ## n  \
 
 
 BOOST_PP_REPEAT(3, TEMPLATE, nil)
 */


#define COMMA_SEP(r, token, i, e) BOOST_PP_COMMA_IF(i) token(e)
#define WRAP(token, ...) BOOST_PP_SEQ_FOR_EACH_I(COMMA_SEP, token, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

/**
 WRAP(decltype, a, b, c)
 
 decltype(a), decltype(b), decltype(c)
 */



#define STRING_TO_CHARS_EXTRACT(z, n, data) \
        BOOST_PP_COMMA_IF(n) get_ch(data, n)

#define STRING_TO_CHARS(STRLEN, STR)  \
        BOOST_PP_REPEAT(STRLEN, STRING_TO_CHARS_EXTRACT, STR)

#define STRING_TO_CHARS2(str) EVAL (DEFER (STRING_TO_CHARS) (sizeof (str), str))


//foo<STRING_TO_CHARS(2, "he")> dd;
