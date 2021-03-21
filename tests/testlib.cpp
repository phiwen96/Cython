#include "headers.hpp"


#



TEST_CASE ("test IF_TOKEN_IS_STRING (token, string, true, false)")
{
    REQUIRE (PH_MACRO_$if_token_is_string (hej, "hej", 24, 0) == 24);
    REQUIRE (PH_MACRO_$if_token_is_string (kuk, "hej", 0, 24) == 24);
    
#define TEST_PARAMS \
    X (hej, "hej", 0, 0)
    
#define X(...) REQUIRE (IF_TOKEN_IS_STRING (__VA_ARGS__) == BOOST_PP_VARIADIC_ELEM (BOOST_PP_VARIADIC_SIZE (__VA_ARGS__) - 1, __VA_ARGS__));
    

//    TEST_PARAMS
    
#undef TEST_PARAMS
#undef X
}




TEST_CASE ("test TOKEN_IS_STRING (x, y)")
{
    REQUIRE (PH_MACRO_$token_is_string (kuk, "kuk") == 1);
    REQUIRE (PH_MACRO_$token_is_string (hej, "phi") == 0);
    
//    #define TEST_TOKEN \
//        X (x) \
//        X (abs) \
//        X (hej då)
//
//    #define X(token) \
//        REQUIRE (TOKEN_IS_STRING (token, BOOST_PP_STRINGIZE (token)));
//
//    TEST_TOKEN
//
//    #undef TEST_TOKEN
//    #undef X
}



namespace ph {
namespace macro {
struct token_is_string
{
#define PH_MACRO_TOKEN_IS_STRING
};
}
}


#define TEXT1(z, n, text) BOOST_PP_COMMA_IF(n) text

#define TEXT2(z, n, text) \
BOOST_PP_COMMA_IF (n) \
      BOOST_PP_REPEAT_ ## z( \
         BOOST_PP_INC(n), \
         TEXT, text [n] \
      )
   /**/

#define TEXT3(x, n) BOOST_PP_CAT (BOOST_PP_REPEAT_, n)(n, BOOST_PP_STRINGIZE (x),  PH_MACRO_$expand(nil))
//#define TEXT4(x) TEXT3 (x, PH_MACRO_$string_size (x))

#define A0(x) x + 2
#define A1(x) x + 1

template <char...>
struct Test{};

#define PRED(d, data) BOOST_PP_TUPLE_ELEM(2, 0, data)

#define OP(d, data) \
   ( \
      BOOST_PP_DEC( \
         BOOST_PP_TUPLE_ELEM(2, 0, data) \
      ), \
      BOOST_PP_ADD_D( \
         d, \
         BOOST_PP_TUPLE_ELEM(2, 1, data), \
         2 \
      ) \
   ) \
   /**/

// increment 'x' by 2 'n' times
#define STRIDE(x, n) BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_WHILE(PRED, OP, (n, x)))

#define AT (str, i)  str[i]


#define TEXT(z, n, text) BOOST_PP_COMMA_IF(n) text

//#define TEMPLATE2(z, n, data)



#define TEMPLATE(z, n, data) \
    IF (data[0] == 'g') (1)
//    EVAL (BOOST_PP_CAT (A, T) (data, 0))
//   BOOST_PP_COMMA_IF(n) \
//   data[n]
   /**/


#include <boost/metaparse/string.hpp>





template <char...> struct foo { };

#define MACR(r, data, elem) BOOST_PP_TUPLE_ELEM (2, 0, elem)
#define PARA(...) BOOST_PP_SEQ_FOR_EACH (MACR, _, __VA_ARGS__)//BOOST_PP_SEQ_FOR_EACH (MACRO, _, __VA_ARGS__)




int test (int, char, string) {}





int main( int argc, char* argv[] ) {
 


    

//    Test<TEXT3("hej", 3)> t1;
//    cout << A1(A2(1)) << endl;
   
//    PH_$empty()
//    IF(1)(cout << "1", cout << "0");
//    cout << STRIDE(10, 2) << endl;
    
   
    
    
    
    IF (1) (cout << "hi" << endl;)
    
    IF_ELSE (1)
        (cout << "1")
        (cout << "0");
    
    
    
    cout << endl;
//    cout << BOOST_TTI_NAMESPACE
    int result = Catch::Session().run( argc, argv );
    string a;
    using ty = int;
//    template <class T>
    bool has_toString = PH_MACRO_$has (c_str()) (a);
    cout << has_toString << endl;
  // global clean-up...

    return result;
}










//TEST_CASE( "Quick check", "[main]" ) {
//
//    REQUIRE( 0 == 0 );
//}



#define DECLARE_DATA_MEMBER3(R,_,TYPE_AND_NAME) \
BOOST_PP_SEQ_ELEM(2,0,TYPE_AND_NAME) //BOOST_PP_CAT(m_,BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME));

#define DEFINE_ACCESSOR3(R,_,TYPE_AND_NAME) \
BOOST_PP_TUPLE_ELEM(2,0,TYPE_AND_NAME) BOOST_PP_SEQ_ELEM(2,1,TYPE_AND_NAME)(){ return BOOST_PP_CAT(m_,BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME)); }

#define MY_MACRO3(TYPES_AND_NAMES) \
BOOST_PP_SEQ_FOR_EACH(DECLARE_DATA_MEMBER3,_,TYPES_AND_NAMES)
//public: \
//BOOST_PP_SEQ_FOR_EACH(DEFINE_ACCESSOR3,_,BOOST_PP_CAT(CREATE_MY_MACRO_PLACEHOLDER_FILLER_0 TYPES_AND_NAMES,_END))


//Heavily "inspired" from BOOST_FUSION_ADAPT_STRUCT
//#define CREATE_MY_MACRO_PLACEHOLDER_FILLER_0(X, Y)  \
//    ((X, Y)) CREATE_MY_MACRO_PLACEHOLDER_FILLER_1
//
//#define CREATE_MY_MACRO_PLACEHOLDER_FILLER_1(X, Y)  \
//    ((X, Y)) CREATE_MY_MACRO_PLACEHOLDER_FILLER_0
//
//#define CREATE_MY_MACRO_PLACEHOLDER_FILLER_0_END
//#define CREATE_MY_MACRO_PLACEHOLDER_FILLER_1_END
//
//#define DECLARE_DATA_MEMBER3(R,_,TYPE_AND_NAME) \
//BOOST_PP_TUPLE_ELEM(2,0,TYPE_AND_NAME) BOOST_PP_CAT(m_,BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME));
//
//#define DEFINE_ACCESSOR3(R,_,TYPE_AND_NAME) \
//BOOST_PP_TUPLE_ELEM(2,0,TYPE_AND_NAME) BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME)(){ return BOOST_PP_CAT(m_,BOOST_PP_TUPLE_ELEM(2,1,TYPE_AND_NAME)); }
//
//#define MY_MACRO3(TYPES_AND_NAMES) \
//BOOST_PP_SEQ_FOR_EACH(DECLARE_DATA_MEMBER3,_,BOOST_PP_CAT(CREATE_MY_MACRO_PLACEHOLDER_FILLER_0 TYPES_AND_NAMES,_END)) \
//public: \
//BOOST_PP_SEQ_FOR_EACH(DEFINE_ACCESSOR3,_,BOOST_PP_CAT(CREATE_MY_MACRO_PLACEHOLDER_FILLER_0 TYPES_AND_NAMES,_END))
