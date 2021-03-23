#pragma once
using namespace std;

namespace _input{
struct Context;
struct State
{
    virtual void process (char const* str, Context& ctx) {}
};

struct Context
{
    State* state {nullptr};
    string input;
    vector <string> outputs;
    string code;
    void process (char const* str);
};


//template <int N>
struct Begin : State
{
    

//    Begin (array<char const*, N>&& states)
//    {
//
//    }
    virtual void process (char const* str, Context& ctx);
};

struct Input : State
{
    virtual void process (char const* str, Context& ctx);
};

struct Output : State
{
    virtual void process (char const* str, Context& ctx);
};

struct Code : State
{
    virtual void process (char const* str, Context& ctx);
};

void Context::process (char const* str)
{
//    cout << "Context::process" << endl;
    state -> process (str, *this);
}


void Begin::process (char const* str, Context& ctx)
{
//    cout << "Begin::process" << endl;
    if (strcmp (str, "--input") == 0)
    {
//        cout << "--input" << endl;
        ctx.state = new Input;
        delete this;
//        ctx.state = static_cast <Input*> (ctx.state);
//        delete this;
    } else if (strcmp (str, "--output") == 0)
    {
        ctx.state = new Output;
        delete this;
        
    } else if (strcmp (str, "--code") == 0)
    {
        ctx.state = new Code;
        delete this;
        
    } else
    {
        throw runtime_error ("");
    }
}

void Input::process (char const* str, Context& ctx)
{
//        cout << "Input::process" << endl;
    if (strcmp (str, "--output") == 0)
    {
        ctx.state = new Output;
        delete this;
        
    } else if (strcmp (str, "--code") == 0)
    {
        ctx.state = new Code;
        delete this;
        
    }
    ctx.input = str;
    ctx.state = new Begin;
//        delete this;
}

void Output::process (char const* str, Context& ctx)
{
//        cout << "Output::process" << endl;
    if (strcmp (str, "--input") == 0)
    {
        ctx.state = new Input;
//            delete this;
    } else if (strcmp (str, "--code") == 0)
    {
        ctx.state = new Code;
        delete this;
        
    } else
    {
        ctx.outputs.push_back (string {str});
    }
}

void Code::process (char const* str, Context& ctx)
{
//        cout << "Output::process" << endl;
    if (strcmp (str, "--input") == 0)
    {
        ctx.state = new Input;
        delete this;
    } else if (strcmp (str, "--output") == 0)
    {
        ctx.state = new Output;
        delete this;
    } else
    {
        ctx.code += str;
    }
}

}

struct inputfsm
{
    string input;
    vector <string> outputs;
    string code;
    
    inputfsm (int argc, char** argv)
    {
        _input::Context ctx {new _input::Begin};
        
        for (char** it = argv + 1; it < argv + argc; ++it)
        {
            ctx.process (*it);
        }
        
        input = ctx.input;
        outputs = ctx.outputs;
        code = ctx.code;
        
        if (input.empty ())
        {
            throw runtime_error ("input path not given");
        }
        
        if (outputs.empty ())
        {
            throw runtime_error ("no output paths where given");
        }
    }
};
