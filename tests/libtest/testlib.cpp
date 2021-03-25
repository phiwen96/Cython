//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
//#include <phulkan_generator/gpu.hpp>
//#include <ph_type_list/ph_type_list.hpp>
// #include <cython/cython_2.hpp>
// #include <ph_macros/macros.hpp>

// using namespace std;




//TEST_CASE ("")
//{
//    Cython2 app {};
//    string input = "";
//
//    string result = "";
//    int nr_of_variables = 0;
//
//    input = R"V0G0N($ (0 i 2)
//{
//@(h){h}
//${h}
//})V0G0N";
//
//    get_result
//    string facit = "h\nh";
//
//    REQUIRE (result == facit);
//
//}





/**
 todo
 "@(last namn) {Wenkel}${last namn}"
 fix so that one space between }$ works as non, } $
 
 
 "$(0 $(variable){x} 3){hej${variable}}"
 let ${${variable}} bli till loop siffran
 */


//#define NEW

// template <char... c>
// void pr (){
//     ((cout << c), ...);
//     cout << endl;
// }

int main( int argc, char* argv[] ) {


	// cout << "hi" << endl;
//    pr< IF_ELSE(TOKEN_IS_STRING(x, "x"))('a','a')('g','g')>();
//    pr<IF_TOKEN_IS_STRING (a, "kuk", A, B)>();
//    cout << macro(begin) << endl;
//    IF_TOKEN_IS_STRING(he, "hej", cout << "hej" << endl, IF_TOKEN_IS_STRING(he, "he", cout << "he" << endl, cout << "no" << endl));
//    pr macro(begin) ();
//    cout << BOOST_PP_STRINGIZE (PH_MACRO)
//    return 0;
//    Cython2 app {};
//    string input = "";
//    string res = "";
//    int nr_of_variables = 0;
//    input =
//R"V0G0N($ (x)
//{
//    2
//        2
//    2
//}
//kuk)V0G0N";
//
//    res = app.process_text (input);
//    cout << res << endl;
//    return 0;
////    get_result
////    get_nr_of_variables
////    auto [name_0, value_0] = app.get_variables()[0];
//
//    return 0;
//    cout << app.process_text ("@(j){name}@(last name) {Wenkel}$(name){Philip ${last ${j}}}") << endl;
//    auto [variable_name, variable_value] = app.get_variables().front();
//    cout << "name:" << variable_name << endl;
//    cout << "value:" << variable_value << endl;
////    cout << app.process_text ("${last namn}") << endl;
////    cout << app.process_text("@(last namn){Wenkel} $(philip ${last name}){en god människa}") << endl;
//    return 0;
    
//    return 0;
//    cout << Cython2{}.process_text ("$(0 x 3){hej}") << endl;
    
//    string input = {"@(namn){Philip}"};
//    cout << Cython2{}.process_text(input) << endl;
//
//    return 0;
//
//    string inp
//{R"V0G0N($(förnamn){Philip}
//${förnamn}
//$(namn){${förnamn} $(efternamn){Wenkel}}
//${namn}
//horans)V0G0N"};
//
//    for (auto i = inp.begin(); i < inp.end(); ++i)
//        ctx.process_text (i);
    
    
    
//    cout << inp << endl;
    
    
// #ifndef NEW
    int result = Catch::Session().run( argc, argv );
    return result;
// #else

//     Cython2 c;
//     cout << c.process_text ("@(j){name}@(last name) {Wenkel}$(name){Philip ${last ${j}}}") << endl;
    
    
    
//     cout << "hi" << endl;
//     return 0;
// #endif
    

}



