cmake_minimum_required (VERSION 3.19.4)

# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")




project (
	cython
	VERSION 1.0
	DESCRIPTION "Very nice project"
	LANGUAGES CXX
)	

include(ProcessorCount)
ProcessorCount(N)
add_compile_definitions(PROCESSOR_COUNT=${N})



if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")

	# Let's ensure -std=g++xx instead of -std=c++xx
	set (CMAKE_CXX_STANDARD 20)
	set (CMAKE_CXX_STANDARD_REQUIRED ON)
	set (CMAKE_CXX_EXTENSIONS ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
	# add_compile_options(-Wall -Wextra -pedantic -Werror)
	add_compile_options (-pthread -Wmissing-attributes -Wno-attributes -Waddress -Wjump-misses-init -Wempty-body -Wdangling-else -Wunsafe-loop-optimizations -Wno-free-nonheap-object -Wtautological-compare -Wduplicated-cond -Wduplicated-branches -Wsuggest-attribute=malloc -Wmissing-noreturn -Wsuggest-attribute=noreturn -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wstring-compare -Wuninitialized -Wunused-value -Wunused-local-typedefs -Wunused-label -Wunused-function -Wunused-but-set-variable -Wunused-but-set-parameter -Wparentheses -Wmissing-braces -Wmissing-attributes -Wmisleading-indentation -Wmain -Wdelete-non-virtual-dtor -Wdeprecated-copy -Wclass-memaccess -Wnon-virtual-dtor -Wreorder -Wno-pessimizing-move -Wno-redundant-move -Woverloaded-virtual -Wno-virtual-move-assign -Wno-placement-new -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wuseless-cast)
	#-Wunused-variable -Wunused-parameter -Wunused-macros -Wjump-misses-init -Wjump-misses-init
	 
	# Let's nicely support folders
	# Let's nicely support folders in IDEs
	set_property (GLOBAL PROPERTY USE_FOLDERS ON)

	# execute_process(COMMAND "echo $<PLATFORM_ID">)
	add_compile_definitions (SYSTEM_NAME=$<PLATFORM_ID>)


	# Testing only available if this is the main app
	# Note this needs to be done in the main CMakeLists
	# since it calls enable_testing, which must be in the
	# main CMakeLists.
	include (CTest)

	set (run cython_app CACHE STRING "run executable")
	set (with_args "" CACHE STRING "run executable with args")


endif()

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
if (NOT TARGET ph_concepts)
	add_subdirectory (dependencies/ph_concepts)
endif ()
if (NOT TARGET ph_coroutines)
	add_subdirectory (dependencies/ph_coroutines)
endif ()
if (NOT TARGET ph_time)
	add_subdirectory (dependencies/ph_time)
endif ()
if (NOT TARGET ph_debug)
	add_subdirectory (dependencies/ph_debug)
endif ()






find_package (Boost 1.74 REQUIRED)

add_subdirectory (src)
add_subdirectory (app)


if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory (tests)
endif()



# add_custom_command
#      OUTPUT ${CMAKE_CURRENT_LIST_DIR}/wrapper
#      COMMAND $(CMAKE_COMMAND)
#           -D CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
#           -P ${CMAKE_CURRENT_SOURCE_DIR}/the_cmake_script.cmake 
#      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/the_cmake_script.cmake 
#      COMMAND Generating wrapper script...
#      VERBATIM
#  )


#  file(WRITE ${CMAKE_CURRENT_LIST_DIR}/run.sh
#  "#!/usr/bin/env bash
#  # Note that './main' is relative from whatever directory you are in
#  # Use just main assuming the install prefix is in your bath
#  # Or use $<TARGET_FILE:main> 
#  # Or maybe ${CMAKE_INSTALL_PREFIX}/bin/main
#  \"$<TARGET_FILE:testlib\">
#  ")

# add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/run.sh
# 	COMMAND 
# )


# git pull origin new   
# git clone --recurse-submodules https://github.com/phiwen96/cython.git  

# cmake -Drun=libtest -Dwith_args="" -S . -B build && cmake --build build --config Debug -j 9 
# cmake -Wno-dev -DCMAKE_CONFIGURATION_TYPES="Debug;Release;Kuk" -S . -B xbuild -GXcode && cmake --open xbuild

# cmake CC=/usr/bin/gcc CXX=/usr/bin/g++ cmake ... -Wno-dev -DCMAKE_CONFIGURATION_TYPES="Debug;Release;Kuk" -S . -B xbuild -GXcode && cmake --open xbuild
# -DCMAKE_C_COMPILER=/usr/local/bin/gcc-10 -DCMAKE_CXX_COMPILER=/usr/local/bin/x86_64-apple-darwin20-g++-10