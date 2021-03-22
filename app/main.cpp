using namespace std;
#include "inputfsm.hpp"
#include "filefsm.hpp"

#if defined (Debug)
auto main (int,  char**) -> int
{
    int argc = 5;
    char** argv = new char*[argc]{new char[]{}, new char[]{"--input"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_pre/1.hpp"}, new char[]{"--output"}, new char[]{"/Users/philipwenkel/GitHub/phan/tests/test_phan_app/testFiles_post/1.hpp"}};
    
#elif defined (Release)
auto main (int argc,  char** argv) -> int
{
#endif

    
        
    
    
    struct file_does_not_exist
    {
        file_does_not_exist ()
        {
            
        }
        file_does_not_exist (filesystem::path const& path)
        {
            throw runtime_error ("path does not exist");
        }
    };
    
    
    
    
    auto [input_file, output_files] = inputfsm (argc, argv);
    
    filefsm <existance::yes <file_does_not_exist>> f (input_file);
    
    
    /**
     takes existance::yes<type> where type constructor const(filesystem::path const&) is called if file does not exist
     takes existance::no<type> where type constructor const(filesystem::path const&) is called if file exists

     */
    
    
    
    
    return 0;
}
