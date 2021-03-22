#pragma once
using namespace std;
#include "common.hpp"
#include <ph_type_list/type_list.hpp>


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


template <template <class...> class SuccessHandler, class PathErrorTag, template <class...> class PathErrorHandler, class FileErrorTag, template <class...> class FileTypeErrorHandler>
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
    
//    using file_type = conditional_t <must_be_file, filetype::file, conditional_t <must_be_folder, filetype::folder, <#class _Then#>>>
    
    
    template <class... T, class... U, class... V>
    static void process (filesystem::path const& path, type_list <T...> sucessHandlerMixins = type_list <> {}, type_list <U...> pathErrorHandlerMixins = type_list <> {}, type_list <V...> fileTypeErrorHandlerMixins = type_list <> {})
    {
        
        using path_error_handler = PathErrorHandler <PathErrorTag, U...>;
        using file_type_error_handler = FileTypeErrorHandler <FileErrorTag, V...>;
    
        if constexpr (path_must_exist)
        {
            using bajs = int;
            if (not PATH_EXISTS)
            {
//                type_list <T...>::
                path_error_handler::error(path);
            }
            
        } else if constexpr (path_must_not_exist)
        {
            if (PATH_EXISTS)
            {
                path_error_handler::error (path);
            }
        }
       
        if constexpr (must_be_file)
        {
            if (not IS_FILE)
            {
                file_type_error_handler::error (path);
            }
            
            SuccessHandler<filetype::file, T...> {path};
            
        } else if constexpr (must_be_folder)
        {
            if (not IS_DIRECTORY)
            {
                file_type_error_handler::error (path);
            }
            
            SuccessHandler<filetype::folder, T...> {path};
        }
        else if constexpr (can_be_any)
        {
            
        }
        
//        using success_handler = SuccessHandler <class>
    }
};






template <template <class...> class SuccessHandler, class PathErrorTag, template <class...> class PathErrorHandler, class FileErrorTag, template <class...> class FileTypeErrorHandler>
struct filefsm
{
//    file::Impl <>* Impl;
    
    filefsm (filesystem::path const& path)
    {
        
        Info <SuccessHandler, PathErrorTag, PathErrorHandler, FileErrorTag, FileTypeErrorHandler>::process (path);
    }
    
    
    template <class... T, class... U, class... V>
    static void yeah (filesystem::path const& path, type_list <T...> sucessHandlerMixins = type_list <> {}, type_list <U...> pathErrorHandlerMixins = type_list <> {}, type_list <V...> fileTypeErrorHandlerMixins = type_list <> {})
    {
        Info <SuccessHandler, PathErrorTag, PathErrorHandler, FileErrorTag, FileTypeErrorHandler>::process (path);
    }
    
    
};
