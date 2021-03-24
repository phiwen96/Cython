using namespace std;
#include "inputfsm.hpp"
#include "common.hpp"
#include <ph_type_list/type_list.hpp>
#include <ph_system_file_path_checker/system_file_path_checker.hpp>
//#include <ph_system_file_path_checker/handles.hpp>
#include <cython/cython.hpp>
#include <cython/cython_2.hpp>




template <class>
struct handle_path_error;

template <>
struct handle_path_error <tag::constraints::path::must_exist>
{
    handle_path_error (filesystem::path const& path)
    {
        throw runtime_error ("given path does not exist on system");
    }
};

template <>
struct handle_path_error <tag::constraints::path::can_exist>
{
    handle_path_error (filesystem::path const& path)
    {
        throw runtime_error ("given path does not exist on system");
    }
};


template <class Tag>
struct handle_file_type_error
{
    handle_file_type_error (filesystem::path const& path)
    {
        if constexpr (is_same_v <Tag, tag::constraints::file_type::must_be_folder>)
        {
            throw runtime_error ("given path refers to a folder");
            
        } else if constexpr (is_same_v <Tag, tag::constraints::file_type::must_be_file>)
        {
            throw runtime_error ("given path refers to a file");
        }
    }
};










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







template <class filetype>
struct does_not_exist
{
    
};

#define NEW

#if defined (Debug)
auto main (int,  char**) -> int
{
    int argc = 8;
    char** argv = new char * [argc] {new char [] {}, new char [] {"--input"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_pre/1.hpp"}, new char [] {"--output"}, new char [] {"/Users/philipwenkel/Documents/testfiles_for_cython/testFiles_post/1.hpp"}, new char[]{"--code"}, new char[]{"@(name){phillo}"}, new char[]{"$(hej){då}"}};
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif
    
#ifndef NEW
    auto [input_path, output_paths, code] = inputfsm (argc, argv);
    using input_reader = system_file_path_checker <InputPathHandler, tag::constraints::path::must_exist, tag::constraints::file_type::can_be_any, handle_path_error, handle_file_type_error>;
    using output_reader = system_file_path_checker <InputPathHandler, tag::constraints::path::can_exist, tag::constraints::file_type::can_be_any, handle_path_error, handle_file_type_error>;

    filesystem::path input = input_reader::process(input_path, type_list <int, char> {}, type_list <string, int> {});
    filesystem::path output = output_reader::process(output_paths.front (), type_list <int, char> {}, type_list <string, int> {});
    Cython cython;
//    cython.process_text (code);
    string output_text = cython.process_text (readFileIntoString(input));
    ofstream f (output);
    f << output_text;
    f.close ();
#if defined (Debug)
    cout << output_text << endl;
//    cout << code << endl;
#endif
    
    
#else
//    string sss = R"V0G0N(
//    $(förnamn){Philip}
//    ${förnamn}
//    $(namn){${förnamn} $(efternamn){Wenkel}}
//    ${namn}
//    horans)V0G0N";
    string sss = R"V0G0N($(förnamn){Philip}
${förnamn}
@(ll){LL}
${ll}
$(namn){${förnamn}}
$(kiss){KISS}
horans)V0G0N";
    cout << Cython2 {}.process_text ( sss )<< endl;
                                     return 0;
#endif
    

    
    return 0;
}
