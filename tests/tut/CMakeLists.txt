# Testing library
ph_fetch (catch2)
ph_define_self (self)
file(GLOB SOURCE_LIST *.cpp)
add_executable (${self} ${SOURCE_LIST})
# target_include_directories(cppcoro PUBLIC )

target_include_directories (${self} PRIVATE ph::color ${PROJECT_SOURCE_DIR}/dependencies/cppcoro/include ${Boost_INCLUDE_DIRS} ph::debug ph::time ph::coroutines ph::concepts Catch2::Catch2 ${PROJECT_NAME} ph::macros ph::type_list const_str)
target_link_libraries (${self} PRIVATE ph::color /Users/philipwenkel/GitHub/Cython/dependencies/cppcoro/build/darwin_x64_clang12.0.5_optimised/lib/libcppcoro.a ${PROJECT_NAME} ph::debug ph::time ph::coroutines ph::concepts ph::macros ph::type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET ${self} PUBLIC PCH ${std_pch} ${boost_pch})
add_test (NAME ${self}test COMMAND ${self}) 
target_compile_definitions(${self} PRIVATE $<IF:$<CONFIG:Debug>,Debug=1,Debug=0> $<IF:$<CONFIG:Release>,Release=1,Release=0> )
# set (ENV{run} "$<TARGET_FILE:teslib>")
# set (ENV{yeah} "echo bajs")


# add_custom_command(TARGET ${self} POST_BUILD COMMAND "osascript -e 'tell app \"Terminal\" to do script \"echo bajs\"'")
# add_custom_command(TARGT ${self} POST_BUILD /bin/sh osascript -e 'tell app \"Terminal\" to do script \"echo bajs\"')

# add_custom_command(TARGET ${self} POST_BUILD COMMAND "osascript -e 'tell app \"Terminal\" to do script \"echo bajs\"'")

# cmake_language(EVAL CODE "add_custom_command(TARGT ${self} POST_BUILD COMMAND osascript -e \'tell app \"Terminal\" to do script \"echo bajs\"\')")
set(where $ENV{${self}})
# add_custom_command(TARGET ${self}
#         POST_BUILD
#         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
# )


ph_run ()

