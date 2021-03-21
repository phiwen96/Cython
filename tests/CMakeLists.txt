# Testing library
ph_fetch (catch2)

add_executable (testlib testlib.cpp)
target_link_libraries (testlib PRIVATE ph_macros Catch2::Catch2)
ph_precompile (TARGET testlib PRIVATE PCH ${std_pch})
add_test (NAME testlibtest COMMAND testlib) 
 






