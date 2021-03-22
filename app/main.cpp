using namespace std;
#include "inputfsm.hpp"
#include "filefsm.hpp"






//template <bool file>
//struct error <file_info::file_must_not_exist, file_info::is_file>
//{
//
//};

template <template <template <class> class> class ExistanceError, class FiltypeError>
struct FileError;


template <>
struct FileError <existance_error::must_exist, filetype_error::any_type>
{
    FileError (filesystem::path const& path)
    {
        
    }
};

template <>
struct FileError <existance_error::must_not_exist, filetype_error::must_be_file>
{
    FileError (filesystem::path const& path)
    {
        
    }
};

template <>
struct FileError <existance_error::must_not_exist, filetype_error::must_be_folder>
{
    FileError (filesystem::path const& path)
    {
        
    }
};



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

    
    
    

    
    
    
    
    auto [input_file, output_files] = inputfsm (argc, argv);
    
    constexpr bool existance = 0;
    filefsm <existance_error::must_exist <does_not_exist>, filetype_error::any_type> f ("hej");
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists

     */
    
    
    
    
    return 0;
}
