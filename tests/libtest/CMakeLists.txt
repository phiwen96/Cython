# Testing library
ph_fetch (catch2)
ph_define_self (self)
add_executable (${self} testlib.cpp)
target_include_directories (${self} PRIVATE ${Boost_INCLUDE_DIRS} Catch2::Catch2 ${PROJECT_NAME} ph_macros ph_type_list const_str)
target_link_libraries (${self} PRIVATE ${PROJECT_NAME} ph_macros ph_type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET ${self} PUBLIC PCH ${std_pch} ${catch_pch} ${boost_pch})
add_test (NAME ${self}test COMMAND ${self}) 
set (ENV{run} "$<TARGET_FILE:teslib>")
set (ENV{yeah} "echo bajs")

add_custom_target(zoo ALL
    COMMAND echo "$<TARGET_FILE:${self}>"#echo "This is ALL target 'zoo', and it depends on ${TEST_FILE}"
    # If the file exists, then commands related to that file won't be executed
    # DONOT let other target depends on the same OUTPUT as current target,
    #   or it may be bad when doing parallel make
#     DEPENDS ${TEST_FILE}

    # to make quotes printable,for example
    VERBATIM
)


ph_run ()

