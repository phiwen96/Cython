using namespace std;
#include "inputfsm.hpp"
#include "common.hpp"
#include <ph_type_list/type_list.hpp>
#include <ph_system_file_path_checker/system_file_path_checker.hpp>
#include <ph_system_file_path_checker/handles.hpp>
#include <cython/cython.hpp>




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
    filesystem::path path;
    
    InputPathHandler (filesystem::path const& path) : path (path)//: text (Cython{}.process_text(readFileIntoString(path))), Mixins {path}...
    {
//        cout << "FILE" << endl;
//        cout << text << endl;
    }
    operator filesystem::path ()
    {
        return path;
    }
};





template <class... Mixins>
struct InputPathHandler <tag::file_type::folder, Mixins...> : Mixins...
{
    filesystem::path path;
    InputPathHandler (filesystem::path const& path) : Mixins {path}..., path (path)
    {
        cout << "FOLDER" << endl;
    }
    operator filesystem::path ()
    {
        return path;
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
    char** argv = new char*[argc]{new char [] {}, new char [] {"--input"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_pre/1.hpp"}, new char [] {"--output"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_post/1.hpp"}};
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif
    

    auto [input_path, output_paths] = inputfsm (argc, argv);
    using input_reader = system_file_path_checker <InputPathHandler, tag::constraints::path::must_exist, tag::constraints::file_type::can_be_any, handle_path_error, handle_file_type_error>;
    using output_reader = system_file_path_checker <InputPathHandler, tag::constraints::path::can_exist, tag::constraints::file_type::can_be_any, handle_path_error, handle_file_type_error>;

    filesystem::path input = input_reader::process(input_path, type_list <int, char> {}, type_list <string, int> {});
    filesystem::path output = output_reader::process(output_paths.front (), type_list <int, char> {}, type_list <string, int> {});
    string output_text = Cython{}.process_text (readFileIntoString(input));
    ofstream f (output);
    f << output_text;
    f.close ();
    
    
    return 0;
}
