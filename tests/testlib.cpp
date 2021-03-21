#include "headers.hpp"



TEST_CASE ("declpaste $(x){foo}")
{
    SECTION ("no spaces")
    {
        GIVEN ("input string")
        {
            string input = {"$(förnamn){Philip}"};
            REQUIRE (Cython{}.process_text(input) == "Philip");
        }
    }
    SECTION ("with spaces")
    {
        GIVEN ("input string")
        {
            string input = {"$  (förnamn)  {Philip}"};
            REQUIRE (Cython{}.process_text(input) == "Philip");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"$\n\n(förnamn)\n\n{Philip}"};
            REQUIRE (Cython{}.process_text(input) == "Philip");
        }
    }
}
TEST_CASE ("decl @(x){foo}")
{
    SECTION ("no spaces")
    {
        GIVEN ("input string")
        {
            Cython app {};
            string input = {"@(förnamn){Philip}"};
            REQUIRE (app.process_text(input) == "");
            
            auto [variable_name, variable_value] = app.get_variables ().front ();
            REQUIRE (variable_name == "förnamn");
            REQUIRE (variable_value == "Philip");
        }
    }
    SECTION ("with spaces")
    {
        GIVEN ("input string")
        {
            string input = {"@  (förnamn)  {Philip}"};
            REQUIRE (Cython{}.process_text(input) == "");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"@\n\n(förnamn)\n\n{Philip}"};
            REQUIRE (Cython{}.process_text(input) == "");
        }
    }
}
TEST_CASE ("comment #{foo}")
{
    SECTION ("no spaces or new lines")
    {
        GIVEN ("input string")
        {
            string input = {"#{Philip}"};
            REQUIRE (Cython{}.process_text(input) == "");
        }
    }
    SECTION ("white lines")
    {
        GIVEN ("input string")
        {
            string input = {"#  {Philip}"};
            REQUIRE (Cython{}.process_text(input) == "");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"#\n\n{Philip}"};
            REQUIRE (Cython{}.process_text(input) == "");
        }
    }
}
TEST_CASE ("test loop $(0 x y){}")
{
    SECTION ("with plain text")
    {
        SECTION ("basic")
        {
            GIVEN ("input string")
            {
                string input = "$(0 x 3){hej}";
                REQUIRE (Cython{}.process_text(input) == "hejhejhej");
            }
        }
        SECTION ("with spaces")
        {
            GIVEN ("input string")
            {
                string input = "$  (0 x 3)  {hej}";
                REQUIRE (Cython{}.process_text(input) == "hejhejhej");
            }
        }
        SECTION ("with new lines")
        {
            GIVEN ("input string")
            {
                string input = "$\n\n(0 x 3)\n\n{hej}";
                REQUIRE (Cython{}.process_text(input) == "hejhejhej");
            }
        }
    }
    SECTION ("using loop variable in plain text")
    {
        GIVEN ("input string")
        {
            string input = "$(0 x 3){hej${x}}";
            REQUIRE (Cython{}.process_text(input) == "hej0hej1hej2");
        }
    }
}
TEST_CASE ("nested pastes")
{
    GIVEN ("a declared variable")
    {
        string input = "@ (last name) {Wenkel}";
        
        AND_GIVEN ("another declared variable")
        {
            input += "@ (j) {name}";
            
            THEN ("use a nested paste to paste the first variable")
            {
                input += "$ {last ${j}}";
                
                REQUIRE (input == "@ (last name) {Wenkel}@ (j) {name}$ {last ${j}}");
                
                Cython app {};
                
                REQUIRE (app.process_text(input) == "Wenkel");
            }
        }
    }
}
TEST_CASE ("declpaste with paste")
{
    SECTION ("declare, then declpaste with a paste")
    {
        GIVEN ("a declared variable")
        {
            Cython app;
            string input = "@ (last name) {Wenkel}";
            REQUIRE (app.process_text (input) == "");
            REQUIRE (app.get_variables().front().first == "last name");
            REQUIRE (app.get_variables().front().second == "Wenkel");
            
            THEN ("declpaste a new variable")
            {
                WHEN ("also pasting the first variable inside the decleration")
                {
                    input += "$(philip ${last name}){en god människa}";
                    REQUIRE (app.process_text(input) == "en god människa");
                    
//                    REQUIRE (Cython{}.process_text(input) == "en god människa");
                }
            }
        }
        
    }
    GIVEN ("input string")
    {
        string inpu = "@(namn)";
    }
}


/**
 todo
 "@(last namn) {Wenkel}${last namn}"
 fix so that one space between }$ works as non, } $
 */

int main( int argc, char* argv[] ) {
 
    
//    Cython app {};
//    cout << app.process_text ("@(j){name}@(last name) {Wenkel}$(name){Philip ${last ${j}}}") << endl;
//    auto [variable_name, variable_value] = app.get_variables().front();
//    cout << "name:" << variable_name << endl;
//    cout << "value:" << variable_value << endl;
////    cout << app.process_text ("${last namn}") << endl;
////    cout << app.process_text("@(last namn){Wenkel} $(philip ${last name}){en god människa}") << endl;
//    return 0;
    
//    return 0;
//    cout << Cython{}.process_text ("$(0 x 3){hej}") << endl;
    
//    string input = {"@(namn){Philip}"};
//    cout << Cython{}.process_text(input) << endl;
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
    
    

    int result = Catch::Session().run( argc, argv );


    return result;
}



