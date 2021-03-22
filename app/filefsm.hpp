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

//struct tag::error::file_type
//{
//    struct must_be_file
//    {
//
//    };
//    struct must_be_folder
//    {
//
//    };
//    struct can_be_any
//    {
//
//    };
//};




struct tag
{
    struct file_type
    {
        struct file
        {
            
        };
        
        struct folder
        {
            
        };
    };
    
    struct error
    {
        struct path
        {
            struct must_exist;
            struct must_not_exist;
        };
        
        struct file_type
        {
            struct must_be_file;
            struct must_be_folder;
            struct can_be_any;
        };
    };
};


template <template <class...> class SuccessHandler, class tag_error_path, template <class...> class PathErrorHandler, class tag_error_file_type, template <class...> class FileTypeErrorHandler>
requires requires (filesystem::path const& path) {
    is_same_v <tag_error_path, tag::error::path::must_exist> or is_same_v <tag_error_path, tag::error::path::must_not_exist>;
    is_same_v <tag_error_file_type, tag::error::file_type::must_be_file> or is_same_v <tag_error_file_type, tag::error::file_type::must_be_folder> or is_same_v <tag_error_file_type, tag::error::file_type::can_be_any>;
    
    PathErrorHandler <tag_error_path>::error (path);
    SuccessHandler <tag::file_type::file> {path};
    SuccessHandler <tag::file_type::folder> {path};
}
struct Info
{
    inline static constexpr bool path_must_exist = is_same_v <tag_error_path, tag::error::path::must_exist>;
    inline static constexpr bool path_must_not_exist = is_same_v <tag_error_path, tag::error::path::must_not_exist>;
    
    inline static constexpr bool must_be_file = is_same_v <tag_error_file_type, tag::error::file_type::must_be_file>;
    inline static constexpr bool must_be_folder = is_same_v <tag_error_file_type, tag::error::file_type::must_be_folder>;
    inline static constexpr bool can_be_any = is_same_v <tag_error_file_type, tag::error::file_type::can_be_any>;
    
//    using file_type = conditional_t <must_be_file, tag::file_type::file, conditional_t <must_be_folder, tag::file_type::folder, <#class _Then#>>>
    
    
    template <class... T, class... U, class... V>
    static void process (filesystem::path const& path, type_list <T...> sucessHandlerMixins = type_list <> {}, type_list <U...> pathErrorHandlerMixins = type_list <> {}, type_list <V...> fileTypeErrorHandlerMixins = type_list <> {})
    {
        
        using path_error_handler = PathErrorHandler <tag_error_path, U...>;
        using file_type_error_handler = FileTypeErrorHandler <tag_error_file_type, V...>;
    
        if constexpr (path_must_exist)
        {
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
            
            SuccessHandler<tag::file_type::file, T...> {path};
            
        } else if constexpr (must_be_folder)
        {
            if (not IS_DIRECTORY)
            {
                file_type_error_handler::error (path);
            }
            
            SuccessHandler<tag::file_type::folder, T...> {path};
            
        } else if constexpr (can_be_any)
        {
            if (IS_FILE)
            {
                SuccessHandler<tag::file_type::file, T...> {path};
                
            } else if (IS_DIRECTORY)
            {
                SuccessHandler<tag::file_type::folder, T...> {path};
                
            } else
            {
                file_type_error_handler::error (path);
            }
        }
        
//        using success_handler = SuccessHandler <class>
    }
};






template <template <class...> class SuccessHandler, class tag_error_path, template <class...> class PathErrorHandler, class tag_error_file_type, template <class...> class FileTypeErrorHandler>
struct system_file_path_checker
{
    
    template <class... T, class... U, class... V>
    system_file_path_checker (filesystem::path const& path, type_list <T...> sucessHandlerMixins = type_list <> {}, type_list <U...> pathErrorHandlerMixins = type_list <> {}, type_list <V...> fileTypeErrorHandlerMixins = type_list <> {})
    {
        
        Info <SuccessHandler, tag_error_path, PathErrorHandler, tag_error_file_type, FileTypeErrorHandler>::process (path);
    }
    
};
