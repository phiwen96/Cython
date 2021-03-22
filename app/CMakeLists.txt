
set (self ${PROJECT_NAME}_app)


add_executable (${self} main.cpp)
target_include_directories (${self} PRIVATE ${Boost_INCLUDE_DIRS} Catch2::Catch2 ${PROJECT_NAME} ph_type_list const_str)
target_link_libraries (${self} PRIVATE ${PROJECT_NAME} ph_type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${catch_pch} ${boost_pch})


install(TARGETS ${self}
          DESTINATION ${CMAKE_INSTALL_BINDIR}
)

