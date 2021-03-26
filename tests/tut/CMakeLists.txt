# Testing library
ph_fetch (catch2)
ph_define_self (self)
file(GLOB SOURCE_LIST *.cpp)
add_executable (${self} ${SOURCE_LIST})
target_include_directories (${self} PRIVATE ${Boost_INCLUDE_DIRS} Catch2::Catch2 ${PROJECT_NAME} ph_macros ph_type_list const_str)
target_link_libraries (${self} PRIVATE ${PROJECT_NAME} ph_macros ph_type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET ${self} PUBLIC PCH ${std_pch} ${boost_pch})
add_test (NAME ${self}test COMMAND ${self}) 
# set (ENV{run} "$<TARGET_FILE:teslib>")
# set (ENV{yeah} "echo bajs")




ph_run ()

