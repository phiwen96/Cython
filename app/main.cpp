using namespace std;
#include "inputfsm.hpp"
#include "common.hpp"
#include <ph_type_list/type_list.hpp>
#include <ph_system_file_path_checker/system_file_path_checker.hpp>
#include <ph_system_file_path_checker/handles.hpp>





//template <bool file>
//struct error <file_info::file_must_not_exist, file_info::is_file>
//{
//
//};












template <class...>
struct InputPathHandler;

template <class... Mixins>
struct InputPathHandler <tag::file_type::file, Mixins...> : Mixins...
{
    string text;
    
    InputPathHandler (filesystem::path const& path) : text (readFileIntoString(path)), Mixins {path}...
    {
        cout << "FILE" << endl;
        cout << text << endl;
    }
};





template <class... Mixins>
struct InputPathHandler <tag::file_type::folder, Mixins...> : Mixins...
{
    
    
    InputPathHandler (filesystem::path const& path) : Mixins {path}...
    {
        cout << "FOLDER" << endl;
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
#define AS_FILE


auto main (int,  char**) -> int
{
    int argc = 5;
#if defined (AS_FILE)
    char** argv = new char*[argc]{new char [] {}, new char [] {"--input"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_pre/1.hpp"}, new char [] {"--output"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_post/1.hpp"}};
#else
    char** argv = new char*[argc]{new char [] {}, new char [] {"--input"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFolders_pre/&(root){philips bibliotek}"}, new char [] {"--output"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFolders_pre/&(root){philips bibliotek}"}};
#endif
    
    
    
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif
    
    
    
//    InputPathErrorHandler<path_error::must_exist> aa ;
//    path_error::must_exist::error<InputPathErrorHandler>("hej");
    
//    path_error::must_exist <InputPathErrorHandler> a;
    
    auto [input_path, output_paths] = inputfsm (argc, argv);
    using input_reader = system_file_path_checker <InputPathHandler, tag::constraints::path::must_exist, tag::constraints::file_type::can_be_any, handle_path_error, handle_file_type_error>;
    input_reader reader (input_path, type_list <int, char> {}, type_list <string, int> {});
    
    
    
    
//    reader.yeah(input_file, type_list <int, char> {}, type_list <string, int> {});
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists
     */
    
    
    
    
    return 0;
}
