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

struct file_error_tags
{
    struct must_be_file
    {
        
    };
    struct must_be_folder
    {
        
    };
    struct can_be_any
    {
        
    };
};

struct path_error_tags {
    struct must_exist;
    struct must_not_exist;
};


template <template <class> class PathErrorHandler, template <class> class SuccessHandler, class PathErrorTag, class FileErrorTag>
requires requires (filesystem::path const& path) {
    is_same_v <PathErrorTag, path_error_tags::must_exist> or is_same_v <PathErrorTag, path_error_tags::must_not_exist>;
    is_same_v <FileErrorTag, file_error_tags::must_be_file> or is_same_v <FileErrorTag, file_error_tags::must_be_folder> or is_same_v <FileErrorTag, file_error_tags::can_be_any>;
    
    PathErrorHandler <PathErrorTag>::error (path);
    SuccessHandler <filetype::file> {path};
    SuccessHandler <filetype::folder> {path};
}
struct Info
{
    inline static constexpr bool path_must_exist = is_same_v <PathErrorTag, path_error_tags::must_exist>;
    inline static constexpr bool path_must_not_exist = is_same_v <PathErrorTag, path_error_tags::must_not_exist>;
    
    inline static constexpr bool must_be_file = is_same_v <FileErrorTag, file_error_tags::must_be_file>;
    inline static constexpr bool must_be_folder = is_same_v <FileErrorTag, file_error_tags::must_be_folder>;
    inline static constexpr bool can_be_any = is_same_v <FileErrorTag, file_error_tags::can_be_any>;

    
    
    Info (filesystem::path const& path)
    {
        if constexpr (path_must_exist)
        {
            if (not PATH_EXISTS)
            {
            PathErrorHandler<PathErrorTag>::error(path);
            }
            
        } else if constexpr (path_must_not_exist)
        {
            if (PATH_EXISTS)
            {
                PathErrorHandler<PathErrorTag>::error (path);
            }
        }
       
        if constexpr (must_be_file)
        {
            if (not IS_FILE)
            {

            }
        } else if constexpr (must_be_folder)
        {
            if (not IS_DIRECTORY)
            {

            }
        }
    }
};






template <template <class> class PathErrorHandler, template <class> class SuccessHandler, class PathErrorTag, class FileErrorTag>
struct filefsm
{
//    file::Impl <>* Impl;
    
    filefsm (filesystem::path const& path)
    {
        
        auto s = Info <PathErrorHandler, SuccessHandler, PathErrorTag, FileErrorTag> (path);
    }
    
    
    
    
};
