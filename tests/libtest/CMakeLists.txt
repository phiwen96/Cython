# Testing library
ph_fetch (catch2)
add_executable (testlib testlib.cpp)
target_include_directories (testlib PRIVATE ${Boost_INCLUDE_DIRS} Catch2::Catch2 ${PROJECT_NAME} ph_type_list const_str)
target_link_libraries (testlib PRIVATE ${PROJECT_NAME} ph_type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET testlib PRIVATE PCH ${std_pch} ${catch_pch} ${boost_pch})
add_test (NAME testlibtest COMMAND testlib) 