#include "headers.hpp"

#define get_result result = app.process_text(input);
#define get_nr_of_variables nr_of_variables = app.get_variables().size();

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
    Cython app {};
    string result = "";
    int nr_of_variables = 0;
    
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
    SECTION ("pasting loop variable in plain text")
    {
        GIVEN ("input string")
        {
            string input = "$(0 x 3){hej${x}}";
            get_result
            get_nr_of_variables
            
            REQUIRE (result == "hej0hej1hej2");
            REQUIRE (nr_of_variables == 0);
        }
    }
    
    SECTION ("2 nested looping")
    {
        string input = "$(0 a 3){$(0 b 2){${a}${b}}}";
        get_result
        get_nr_of_variables
        
        REQUIRE (result == "000110112021");
        REQUIRE (nr_of_variables == 0);
    }
}

TEST_CASE ("declaring variables")
{
    Cython app {};
    string input = "";
    
    string result = "";
    int nr_of_variables = 0;
    
    
    GIVEN ("a declared variable")
    {
        input = "@ (last name) {bajs}";
        
        THEN ("we snould get an empty string and 1 declared variable back")
        {
//            Cython app {};
            string result = app.process_text (input);
            int nr_of_variables = app.get_variables().size();
            auto [name, value] = app.get_variables ().front ();
            app.clear_variables ();
            
            
            REQUIRE (result == "");
            REQUIRE (nr_of_variables == 1);
            REQUIRE (name == "last name");
            REQUIRE (value == "bajs");
        }
        
        AND_THEN ("pasting it")
        {
            WHEN ("declaring a new one")
            {
                SECTION ("in the paranthesis")
                {
                    input += "@ (gillar $ {last name}){true}";
                    
                    THEN ("gillar bajs = true")
                    {
                        result = app.process_text (input);
                        nr_of_variables = app.get_variables().size();
                        auto [name_0, value_0] = app.get_variables()[0];
                        auto [name_1, value_1] = app.get_variables()[1];
                        app.clear_variables ();
                        
                        REQUIRE (result == "");
                        REQUIRE (nr_of_variables == 2);
                        REQUIRE (name_0 == "last name");
                        REQUIRE (value_0 == "bajs");
                        REQUIRE (name_1 == "gillar bajs");
                        REQUIRE (value_1 == "true");
                    }
                }
                
                SECTION ("in the curly brackets")
                {
                    input += "@ (gillar){$ {last name} true}";
                    
                    THEN ("gillar = bajs true")
                    {
                        result = app.process_text (input);
                        nr_of_variables = app.get_variables().size();
                        auto [name_0, value_0] = app.get_variables()[0];
                        auto [name_1, value_1] = app.get_variables()[1];
                        app.clear_variables ();
                        
                        REQUIRE (result == "");
                        REQUIRE (nr_of_variables == 2);
                        REQUIRE (name_0 == "last name");
                        REQUIRE (value_0 == "bajs");
                        REQUIRE (name_1 == "gillar");
                        REQUIRE (value_1 == "bajs true");
                    }
                }
            }
            
            input += "${last name}";
            REQUIRE (app.process_text(input) == "bajs");
        }
        
        AND_WHEN ("declaring the same variable again but with a different value")
        {
            input += "@ (last name) {Wenkel}";
            
            THEN ("we should get the same variable but now with a another value")
            {
                Cython app {};
                string result = app.process_text (input);
                int nr_of_variables = app.get_variables ().size ();
                auto [name, value] = app.get_variables ().front ();
                app.clear_variables ();
                
                REQUIRE (result == "");
                REQUIRE (nr_of_variables == 1);
                REQUIRE (name == "last name");
                REQUIRE (value == "Wenkel");
                
            }
        }
        
        AND_GIVEN ("another declared variable")
        {
            input += "@ (j) {name}";
            
            THEN ("we should still get an empty string, but this time with 2 variables")
            {
                result = app.process_text (input);
                nr_of_variables = app.get_variables().size();
                auto [name_0, value_0] = app.get_variables()[0];
                auto [name_1, value_1] = app.get_variables()[1];
                app.clear_variables ();
                
                
                REQUIRE (result == "");
                REQUIRE (nr_of_variables == 2);
                REQUIRE (name_0 == "last name");
                REQUIRE (value_0 == "bajs");
                REQUIRE (name_1 == "j");
                REQUIRE (value_1 == "name");
                
                
                AND_WHEN ("adding a nested paste to use these two variables")
                {
                    input += "$ {last ${j}}";
                    
                    THEN ("the paste would first become $ {last name}")
                    {
                        AND_THEN ("the whole paste would just become bajs")
                        {
                            REQUIRE (app.process_text(input) == "bajs");
                        }
                    }
                }
            }
        }
    }
}



TEST_CASE ("declpasting")
{
    Cython app {};
    string input = "";
    
    string result = "";
    int nr_of_variables = 0;
    
    WHEN ("declpasting during a variable decleration")
    {
        SECTION ("in the paranthesis")
        {
            input = "@($(name){Philip} Wenkel){är bäst}";
            
            THEN ("we should get")
            {
                get_result
                get_nr_of_variables
                auto [name_0, value_0] = app.get_variables()[0];
                auto [name_1, value_1] = app.get_variables()[1];
                
                REQUIRE (result == "");
                REQUIRE (nr_of_variables == 2);
                REQUIRE (name_0 == "name");
                REQUIRE (value_0 == "Philip");
                REQUIRE (name_1 == "Philip Wenkel");
                REQUIRE (value_1 == "är bäst");
            }
        }
        
        SECTION ("in the curly brackets")
        {
            input = "@(person){$(name){Philip} är bäst}";
            
            THEN ("we should get")
            {
                get_result
                get_nr_of_variables
                auto [name_0, value_0] = app.get_variables()[0];
                auto [name_1, value_1] = app.get_variables()[1];
                
                REQUIRE (result == "");
                REQUIRE (nr_of_variables == 2);
                REQUIRE (name_0 == "name");
                REQUIRE (value_0 == "Philip");
                REQUIRE (name_1 == "person");
                REQUIRE (value_1 == "Philip är bäst");
            }
        }
    }
    
    WHEN ("declpasting during a declpaste")
    {
        SECTION ("inside the paranthesis")
        {
            input = "$(Philip $(last name){Wenkel}){is the best}";
            
            get_result
            get_nr_of_variables
            auto [name_0, value_0] = app.get_variables()[0];
            auto [name_1, value_1] = app.get_variables()[1];
            
            REQUIRE (result == "is the best");
            REQUIRE (nr_of_variables == 2);
            REQUIRE (name_0 == "last name");
            REQUIRE (value_0 == "Wenkel");
            REQUIRE (name_1 == "Philip Wenkel");
            REQUIRE (value_1 == "is the best");
        }
        
        SECTION ("inside the curly brackets")
        {
            input = "$(name){Philip $(last name){Wenkel}}";
            
            get_result
            get_nr_of_variables
            auto [name_0, value_0] = app.get_variables()[0];
            auto [name_1, value_1] = app.get_variables()[1];
            
            REQUIRE (result == "Philip Wenkel");
            REQUIRE (nr_of_variables == 2);
            REQUIRE (name_0 == "last name");
            REQUIRE (value_0 == "Wenkel");
            REQUIRE (name_1 == "name");
            REQUIRE (value_1 == "Philip Wenkel");
        }
    }
    
    WHEN ("declpasting during a loop")
    {
        SECTION ("inside paranthesis")
        {
            SECTION ("as first number")
            {
//                input = "$($(variable){0} x 3){hej${x}}";
//                
//                get_result
//                get_nr_of_variables
//                auto [name_0, value_0] = app.get_variables()[0];
//                
//                REQUIRE (result == "hej0hej1hej2");
//                REQUIRE (nr_of_variables == 1);
//                REQUIRE (name_0 == "variable");
//                REQUIRE (value_0 == "x");
            }
            SECTION ("as loop variable")
            {
                input = "$(0 $(variable){x} 3){hej${variable}2${variable}}";
                
                get_result
                get_nr_of_variables
                auto [name_0, value_0] = app.get_variables()[0];
                
                REQUIRE (result == "hejx2xhejx2xhejx2x");
                REQUIRE (nr_of_variables == 1);
                REQUIRE (name_0 == "variable");
                REQUIRE (value_0 == "x");
            }
            SECTION ("as loop variable")
            {
                input = "$(0 $(variable){x} 3){hej$(k){K}$(kk){KK}${variable}k${x}}";
                
                get_result
                get_nr_of_variables
                auto [name_0, value_0] = app.get_variables()[0];
                
                REQUIRE (result == "hejKKKxk0hejKKKxk1hejKKKxk2");
                REQUIRE (nr_of_variables == 3);
                REQUIRE (name_0 == "variable");
                REQUIRE (value_0 == "x");
            }
            SECTION ("as loop variable")
            {
                input = "$(0 $(variable){x} 3){@(j${x}){aa}hej#{kuk}${x}}";
                
                get_result
                get_nr_of_variables
                auto [name_0, value_0] = app.get_variables()[0];
                
                REQUIRE (result == "hej0hej1hej2");
                REQUIRE (nr_of_variables == 4);
                REQUIRE (name_0 == "variable");
                REQUIRE (value_0 == "x");
            }
            SECTION ("as second number")
            {
                
            }
        }
        
        SECTION ("inside curly brackets")
        {
            input = "$(Philip $(last name){Wenkel}){is the best}";
            
            get_result
            get_nr_of_variables
            auto [name_0, value_0] = app.get_variables()[0];
            auto [name_1, value_1] = app.get_variables()[1];
            
            REQUIRE (result == "is the best");
            REQUIRE (nr_of_variables == 2);
            REQUIRE (name_0 == "last name");
            REQUIRE (value_0 == "Wenkel");
            REQUIRE (name_1 == "Philip Wenkel");
            REQUIRE (value_1 == "is the best");
        }
    }
    
    
    
    GIVEN ("a declpaste")
    {
        
        input = "$(first name){Philip}";
        
        THEN ("we should get text: Philip; variable:name: first name; variable:value: Philip")
        {
            result = app.process_text (input);
            nr_of_variables = app.get_variables().size();
            auto [name, value] = app.get_variables().front();
            
            
            REQUIRE(result == "Philip");
            REQUIRE(nr_of_variables == 1);
            REQUIRE(name == "first name");
            REQUIRE(value == "Philip");
        }
        
        
        
       
        AND_GIVEN ("another declpaste")
        {
            WHEN ("the new one has the same variable name")
            {
                input += "$(first name){Kalle}";
                
                THEN ("we should get both pastes but still just get one variable")
                {
                    result = app.process_text (input);
                    nr_of_variables = app.variables.size();
                    auto [name, value] = app.get_variables().front();
                    
                    REQUIRE(result == "PhilipKalle");
                    REQUIRE(nr_of_variables == 1);
                    REQUIRE(name == "first name");
                    REQUIRE(value == "Kalle");;
                }
            }
            WHEN ("the new one has a different variable name")
            {
                input += "$(age){24}";
                THEN ("we should get both pastes and 2 variables")
                {
                    result = app.process_text (input);
                    nr_of_variables = app.variables.size();
                    auto [name_0, value_0] = app.get_variables()[0];
                    auto [name_1, value_1] = app.get_variables()[1];
                    
                    REQUIRE(result == "Philip24");
                    REQUIRE(nr_of_variables == 2);
                    REQUIRE(name_0 == "first name");
                    REQUIRE(value_0 == "Philip");;
                    REQUIRE(name_1 == "age");
                    REQUIRE(value_1 == "24");
                }
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

TEST_CASE ("pasting")
{
    Cython app {};
    string input = "";
    
    string result = "";
    int nr_of_variables = 0;
    
    GIVEN ("a variable")
    {
        input = "@(x){12}";
        THEN ("pasting it once")
        {
            input += "${x}";
            get_result
            REQUIRE (result == "12");
        }
        
        THEN ("pasting it twice")
        {
            input += "${x}${x}";
            get_result
            REQUIRE (result == "1212");
        }
        
    }
}

TEST_CASE ("")
{
    Cython app {};
    string input = "";
    
    string result = "";
    int nr_of_variables = 0;
    
    input = "@(k){K}\n${k}";
    get_result
    REQUIRE (result == "K");
}


/**
 todo
 "@(last namn) {Wenkel}${last namn}"
 fix so that one space between }$ works as non, } $
 
 
 "$(0 $(variable){x} 3){hej${variable}}"
 let ${${variable}} bli till loop siffran
 */

int main( int argc, char* argv[] ) {
 
    
//    Cython app {};
//    string input = "";
//    string res = "";
//    int nr_of_variables = 0;
//    input = "$(0 $(variable){x} 3){hej}";
//    res = app.process_text (input);
//    cout << res << endl;
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



