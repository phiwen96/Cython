using namespace std;
#include "inputfsm.hpp"
#include "filefsm.hpp"
#include "common.hpp"






//template <bool file>
//struct error <file_info::file_must_not_exist, file_info::is_file>
//{
//
//};

template <class>
struct InputPathErrorHandler;

template <>
struct InputPathErrorHandler <path_error::must_exist>
{
    InputPathErrorHandler (filesystem::path const& path)
    {
        throw runtime_error ("given path does not exist on system");
    }
};

template <class>
struct OutputPathErrorHandler;

template <>
struct OutputPathErrorHandler <path_error::must_not_exist>
{
    OutputPathErrorHandler (filesystem::path const& path)
    {
        throw runtime_error ("given path already exist on system");
    }
};



template <class>
struct InputPathHandler;

template <>
struct InputPathHandler <filetype::file>
{
    string text;
    
    InputPathHandler (filesystem::path const& path) : text (readFileIntoString(path))
    {
        cout << text << endl;
    }
};

template <>
struct InputPathHandler <filetype::folder>
{
    InputPathHandler (filesystem::path const& path)
    {
        
    }
};




//template <>
//struct InputPathErrorHandler <path_error::must_not_exist, filetype_error::must_be_file>
//{
//    InputPathErrorHandler (filesystem::path const& path)
//    {
//
//    }
//};
//
//template <>
//struct InputPathErrorHandler <path_error::must_not_exist, filetype_error::must_be_folder>
//{
//    InputPathErrorHandler (filesystem::path const& path)
//    {
//
//    }
//};



template <class filetype>
struct does_not_exist
{
    
};

#if defined (Debug)
auto main (int,  char**) -> int
{
    int argc = 5;
    char** argv = new char*[argc]{new char[]{}, new char[]{"--input"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/1.hpp"}, new char[]{"--output"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/1.hpp"}};
    
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif

    
    
    
//    InputPathErrorHandler<path_error::must_exist> aa ;
//    path_error::must_exist::error<InputPathErrorHandler>("hej");
    
//    path_error::must_exist <InputPathErrorHandler> a;
    
    auto [input_file, output_files] = inputfsm (argc, argv);
    
    constexpr bool existance = 0;
    
    
    using input_reader = filefsm <path_error::must_exist, InputPathErrorHandler, InputPathHandler>;
    input_reader reader (input_file);
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists

     */
    
    
    
    
    return 0;
}
