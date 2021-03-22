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

#define TRANSITION(...) ctx.Impl = transition <__VA_ARGS__> ();



struct filetype {
    struct file
    {
        
    };
    struct folder
    {
        
    };
};

struct filetype_error
{
    struct must_be_file
    {
        
    };
    struct must_be_folder
    {
        
    };
    struct any_type
    {
        
    };
};

struct existance_error {
//    inline static constexpr bool yes = true;
//    inline static constexpr bool no = true;
    struct must_exist
    {
        template <template <class> class ErrorImpl>
        static void error (filesystem::path const& path)
        {
            ErrorImpl <must_exist> e (path);
        }
    };
    
    template <template <class filetype> class Error>
    struct must_not_exist
    {
        template <class filetype>
        static void error (filesystem::path const& path)
        {
            Error <filetype> e (path);
        }
    };
    
};




namespace file{
struct Context;

template <bool, class...>
struct Impl;

//template <>
//struct Impl <>
//{
//    virtual void PROCESS {}
//};

struct Context
{
//    Impl<>* Impl {nullptr};
//
//    Context& process (filesystem::path const& inputPath)
//    {
//        Impl -> process (inputPath, *this);
//        return *this;
//    }
//
//    template <class... T>
//    void transition ()
//    {
//        delete Impl;
//        Impl = new Impl <T...>;
//    }
};
/**
 takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
 takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists
 */
template <template <template <template <template <class> class> class ExistanceError, class FiltypeError> class> class Error>
struct Info;


//template <template <template <template <template <template <class> class> class ExistanceError, class FiltypeError> class> class Error2> class Error>
//struct Info //<existance_error::must_exist <Error>>
//{
//    Info (filesystem::path const& path);
//};
//
//template <template <class filetype> class ExistanceError, class FiletypeError, class... Mixins>
//struct Info <existance_error::must_not_exist <ExistanceError>, FiletypeError, Mixins...>
//{
//    Info (filesystem::path const& path);
//};




//template <class... Mixins>
//struct Impl <type::file, Mixins...>
//{
//    Impl (filesystem::path const& path);
//};
//
//template <class... Mixins>
//struct Impl <type::folder, Mixins...>
//{
//    Impl (filesystem::path const& path);
//};



//template <template <class filetype> class ExistanceError, class FiletypeError, class... Mixins>
//Info<existance_error::must_exist <ExistanceError>, FiletypeError, Mixins...>::Info (filesystem::path const& path)
//{
//
//
//    if (not PATH_EXISTS)
//    {
//        existance_error::must_exist <ExistanceError>::error (path);
////        existance::must_exist <Error> f (path);
//    }
//
//    if (IS_DIRECTORY)
//    {
////        ctx.transition <existance::yes, type::folder> ();
////        Impl <type::folder, Mixins...> s (path);
////        delete this;
//
//    } else if (IS_FILE)
//    {
////        Impl <type::file, Mixins...> s (path);
////        ctx.transition <existance::yes, type::file> ();
////        Impl <existance::yes <T...>, type::file> s (path);
//
//    } else
//    {
//        THROW_UNKNOWN_FILE_TYPE
//    }
//}
//
//
//template <template <class filetype> class ExistanceError, class FiletypeError, class... Mixins>
//Info<existance_error::must_not_exist <ExistanceError>, FiletypeError, Mixins...>::Info (filesystem::path const& path)
//{
//    if (PATH_EXISTS)
//    {
//        THROW_PATH_ALREADY_EXISTS
//    }
//
//    if (IS_DIRECTORY)
//    {
////        Impl <0, type::folder, Mixins...> s (path);
////        Impl <existance::no <T...>, type::folder> s (path);
//
//    } else if (IS_FILE)
//    {
////        Impl <0, type::file, Mixins...> s (path);
////        Impl <existance::no <T...>, type::file> s (path);
//
//    } else
//    {
//        THROW_UNKNOWN_FILE_TYPE
//    }
//}


//template <class... Mixins>
//Impl <type::file, Mixins...>::Impl (filesystem::path const& path)
//{
////    string input = readFileIntoString (inputPath);
////        ofstream outputFile (outputPath);
////
////        if (!outputFile.is_open ())
////            throw runtime_error ("could not open file " + outputPath.string());
////
////        outputFile << p.process (input);
////        outputFile.close ();
//}
//
//template <class... Mixins>
//Impl <type::folder, Mixins...>::Impl (filesystem::path const& path)
//{
//
//}


}

struct fileinfo
{
    filesystem::path path;
    
};







template <template <template <template <template <class> class> class ExistanceError, class FiltypeError> class> class>
struct filefsm
{
//    file::Impl <>* Impl;
    
    filefsm (filesystem::path const& path)
    {
        
//        auto s = file::Info<T...>(path);
    }
    
    
    
    
};
