#pragma once
using namespace std;
#include "common.hpp"


#define PROCESS process (filesystem::path const& path, Context& ctx)



#define PATH_EXISTS filesystem::exists (path)

#define THROW_PATH_ALREADY_EXISTS throw runtime_error ("path already exists!");
#define THROW_PATH_DOES_NOT_EXIST throw runtime_error ("path does not exist!");
#define THROW_UNKNOWN_FILE_TYPE throw runtime_error ("unknown filetype");

#define IS_DIRECTORY filesystem::is_directory (path)
#define IS_FILE filesystem::is_regular_file (path)

#define TRANSITION(...) ctx.state = transition <__VA_ARGS__> ();



struct type {
    struct file
    {
        
    };
    struct folder
    {
        
    };
};

struct existance {
//    inline static constexpr bool yes = true;
//    inline static constexpr bool no = true;
    template <class... T>
    struct yes : T...
    {
        yes (){}
        yes (filesystem::path const& path) : T(path)... {
            if constexpr (sizeof... (T) == 0)
            {
                THROW_PATH_DOES_NOT_EXIST
            }
        }
    };
    
    template <class... T>
    struct no : T...
    {
        no (){}
        no (filesystem::path const& path) : T(path)... {
            if constexpr (sizeof... (T) == 0)
            {
                THROW_PATH_ALREADY_EXISTS
            }
        }
    };
    
};




namespace file{
struct Context;

template <class...>
struct State;

//template <>
//struct State <>
//{
//    virtual void PROCESS {}
//};

struct Context
{
//    State<>* state {nullptr};
//
//    Context& process (filesystem::path const& inputPath)
//    {
//        state -> process (inputPath, *this);
//        return *this;
//    }
//
//    template <class... T>
//    void transition ()
//    {
//        delete state;
//        state = new State <T...>;
//    }
};


template <class... T>
struct State <existance::yes <T...>>
{
    State (filesystem::path const& path);
};

template <class... T>
struct State <existance::no <T...>>
{
    State (filesystem::path const& path);
};



template <class exist>
struct State <exist, type::file>
{
    State (filesystem::path const& path);
};

template <class exist>
struct State <exist, type::folder>
{
    State (filesystem::path const& path);
};



template <class... T>
State<existance::yes <T...>>::State (filesystem::path const& path)
{
    if (not PATH_EXISTS)
    {
        existance::yes <T...> f (path);
    }
    
    if (IS_DIRECTORY)
    {
//        ctx.transition <existance::yes, type::folder> ();¨
        State <existance::yes <T...>, type::folder> s (path);
        delete this;
        
    } else if (IS_FILE)
    {
//        ctx.transition <existance::yes, type::file> ();
        State <existance::yes <T...>, type::file> s (path);
        
    } else
    {
        THROW_UNKNOWN_FILE_TYPE
    }
}


template <class... T>
State<existance::no <T...>>::State (filesystem::path const& path)
{
    if (PATH_EXISTS)
    {
        THROW_PATH_ALREADY_EXISTS
    }
    
    if (IS_DIRECTORY)
    {
        State <existance::no <T...>, type::folder> s (path);

    } else if (IS_FILE)
    {
        State <existance::no <T...>, type::file> s (path);

    } else
    {
        THROW_UNKNOWN_FILE_TYPE
    }
}

template <class exist>
State <exist, type::file>::State (filesystem::path const& path)
{
//    string input = readFileIntoString (inputPath);
//        ofstream outputFile (outputPath);
//
//        if (!outputFile.is_open ())
//            throw runtime_error ("could not open file " + outputPath.string());
//
//        outputFile << p.process (input);
//        outputFile.close ();
}

template <class exist>
State <exist, type::folder>::State (filesystem::path const& path)
{
    
}


}

struct fileinfo
{
    filesystem::path path;
    
};







template <class... T>
struct filefsm
{
//    file::State <>* state;
    
    filefsm (filesystem::path const& path)
    {
        auto s = file::State<T...>(path);
    }
    
    
    
    
};
