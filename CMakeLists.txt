cmake_minimum_required (VERSION 3.19.4)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")


project (
	cython
	VERSION 1.0
	DESCRIPTION "Very nice project"
	LANGUAGES CXX
)	


add_subdirectory (cmake/phake)


if (NOT TARGET const_str)
	add_subdirectory (dependencies/const_str)
endif ()
if (NOT TARGET ph_type_list)
	add_subdirectory (dependencies/ph_type_list)
endif ()
if (NOT TARGET ph_system_file_path_checker)
	add_subdirectory (dependencies/ph_system_file_path_checker)
endif ()
if (NOT TARGET ph_macros)
	add_subdirectory (dependencies/ph_macros)
endif ()


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	
	# Let's ensure -std=g++xx instead of -std=c++xx
	set (CMAKE_CXX_STANDARD 20)
	set (CMAKE_CXX_STANDARD_REQUIRED ON)
	set (CMAKE_CXX_EXTENSIONS ON)

	# Let's nicely support folders in IDEs
	set_property (GLOBAL PROPERTY USE_FOLDERS ON)



	# Testing only available if this is the main app
	# Note this needs to be done in the main CMakeLists
	# since it calls enable_testing, which must be in the
	# main CMakeLists.
	include (CTest)



   # Docs only available if this is the main app
   	# find_package (Doxygen)

	# if (Doxygen_FOUND)
	# 	add_subdirectory (docs)
   	# else()
    #  		message(STATUS "Doxygen not found, not building docs")
   	# endif()


endif()

find_package (Boost 1.74 REQUIRED)

add_subdirectory (src)
add_subdirectory (app)


if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory (tests)
endif()






# git pull origin new   
# git clone --recurse-submodules https://github.com/phiwen96/cython.git  