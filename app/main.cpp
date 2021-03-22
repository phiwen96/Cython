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
struct InputPathError;

template <>
struct InputPathError <existance_error::must_exist>
{
    InputPathError (filesystem::path const& path)
    {
        throw runtime_error ("given path does not exist on system");
    }
};

template <class>
struct OutputPathError;

template <>
struct OutputPathError <existance_error::must_not_exist>
{
    OutputPathError (filesystem::path const& path)
    {
        throw runtime_error ("given path already exist on system");
    }
};



template <class>
struct ReadInput;

template <>
struct ReadInput <filetype::file>
{
    string text;
    
    ReadInput (filesystem::path const& path) : text (readFileIntoString(path))
    {
        cout << text << endl;
    }
};

template <>
struct ReadInput <filetype::folder>
{
    ReadInput (filesystem::path const& path)
    {
        
    }
};




//template <>
//struct InputPathError <existance_error::must_not_exist, filetype_error::must_be_file>
//{
//    InputPathError (filesystem::path const& path)
//    {
//
//    }
//};
//
//template <>
//struct InputPathError <existance_error::must_not_exist, filetype_error::must_be_folder>
//{
//    InputPathError (filesystem::path const& path)
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

    
    
    
//    InputPathError<existance_error::must_exist> aa ;
//    existance_error::must_exist::error<InputPathError>("hej");
    
//    existance_error::must_exist <InputPathError> a;
    
    auto [input_file, output_files] = inputfsm (argc, argv);
    
    constexpr bool existance = 0;
    
    
    using input_reader = filefsm <existance_error::must_exist, InputPathError, ReadInput>;
    input_reader reader (input_file);
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists

     */
    
    
    
    
    return 0;
}
