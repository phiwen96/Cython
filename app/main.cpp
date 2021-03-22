using namespace std;
#include "inputfsm.hpp"
#include "filefsm.hpp"
#include "common.hpp"
#include <ph_type_list/type_list.hpp>






//template <bool file>
//struct error <file_info::file_must_not_exist, file_info::is_file>
//{
//
//};

template <class>
struct InputPathErrorHandler;

template <>
struct InputPathErrorHandler <path_error_tags::must_exist>
{
    static void error (filesystem::path const& path)
    {
        throw runtime_error ("given path does not exist on system");
    }
};


template <class>
struct OutputPathErrorHandler;

template <>
struct OutputPathErrorHandler <path_error_tags::must_not_exist>
{
    static void error (filesystem::path const& path)
    {
        throw runtime_error ("given path already exist on system");
    }
};


template <class>
struct InputFileTypeErrorHandler
{
    static void error (filesystem::path const& path)
    {
        throw runtime_error ("given path already exist on system");
    }
};

template <class>
struct OutputFileTypeErrorHandler
{
    static void error (filesystem::path const& path)
    {
        throw runtime_error ("given path already exist on system");
    }
};








template <class, class...>
struct InputPathHandler;

template <class... Mixins>
struct InputPathHandler <filetype::file, Mixins...>
{
    string text;
    
    InputPathHandler (filesystem::path const& path) : text (readFileIntoString(path))
    {
        cout << text << endl;
    }
};




template <class... Mixins>
struct InputPathHandler <filetype::folder, Mixins...>
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
    char** argv = new char*[argc]{new char [] {}, new char [] {"--input"}, new char [] {"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/1.hpp"}, new char [] {"--output"}, new char [] {"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/1.hpp"}};
    
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif
    
    
    
//    InputPathErrorHandler<path_error::must_exist> aa ;
//    path_error::must_exist::error<InputPathErrorHandler>("hej");
    
//    path_error::must_exist <InputPathErrorHandler> a;
    
    auto [input_file, output_files] = inputfsm (argc, argv);
    constexpr bool existance = 0;
    
    
    
    
    using input_reader = filefsm <InputPathHandler, path_error_tags::must_exist, InputPathErrorHandler, file_error_tags::can_be_any, InputFileTypeErrorHandler>;
    input_reader reader (input_file);
    
    
    reader.yeah(input_file, type_list <int, char> {}, type_list <string, int> {});
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists
     */
    
    
    
    
    return 0;
}
