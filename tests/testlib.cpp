#include "headers.hpp"

#define CONTEXT(x) Context x{.parent = nullptr, .declaredVariables = variables, .state = new STATE ("begin")}
#define VARIABLES(x) vector <pair <string, string>> x

struct TestApp
{
    vector <pair <string, string>> variables;
    
    Context ctx
    {
        .parent = nullptr,
        .declaredVariables = variables,
        .state = new STATE ("begin")
    };
    
    string process (string const& str)
    {
        for (auto i = str.begin(); i < str.end(); ++i)
            ctx.process (i);
        
        return ctx.result;
    }
    
    ~TestApp ()
    {
        ctx.result.clear ();
    }
    
};

TEST_CASE ("declpaste $(x){foo}")
{
    SECTION ("no spaces")
    {
        GIVEN ("input string")
        {
            string input = {"$(förnamn){Philip}"};
            REQUIRE (TestApp{}.process(input) == "Philip");
        }
    }
    SECTION ("with spaces")
    {
        GIVEN ("input string")
        {
            string input = {"$  (förnamn)  {Philip}"};
            REQUIRE (TestApp{}.process(input) == "Philip");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"$\n\n(förnamn)\n\n{Philip}"};
            REQUIRE (TestApp{}.process(input) == "Philip");
        }
    }
}
TEST_CASE ("decl @(x){foo}")
{
    SECTION ("no spaces")
    {
        GIVEN ("input string")
        {
            string input = {"@(förnamn){Philip}"};
            REQUIRE (TestApp{}.process(input) == "");
        }
    }
    SECTION ("with spaces")
    {
        GIVEN ("input string")
        {
            string input = {"@  (förnamn)  {Philip}"};
            REQUIRE (TestApp{}.process(input) == "");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"@\n\n(förnamn)\n\n{Philip}"};
            REQUIRE (TestApp{}.process(input) == "");
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
            REQUIRE (TestApp{}.process(input) == "");
        }
    }
    SECTION ("white lines")
    {
        GIVEN ("input string")
        {
            string input = {"#  {Philip}"};
            REQUIRE (TestApp{}.process(input) == "");
        }
    }
    SECTION ("with new lines")
    {
        GIVEN ("input string")
        {
            string input = {"#\n\n{Philip}"};
            REQUIRE (TestApp{}.process(input) == "");
        }
    }
}




int main( int argc, char* argv[] ) {
 
    vector <pair <string, string>> variables;
    
    Context ctx
    {
        .parent = nullptr,
        .declaredVariables = variables,
        .state = new STATE ("begin")
    };
    
//    string input = {"@(namn){Philip}"};
//    cout << TestApp{}.process(input) << endl;
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
//        ctx.process (i);
    
    
    
//    cout << inp << endl;
    
    

    int result = Catch::Session().run( argc, argv );


    return result;
}



