
set (self ${PROJECT_NAME}_app)

file (GLOB HEADER_LIST *.hpp)




add_executable (${self} main.cpp)
foreach (file ${HEADER_LIST})
    target_sources (${self} PRIVATE ${file})
endforeach ()

set_target_properties(${self} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_include_directories (${self} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS} Catch2::Catch2 ${PROJECT_NAME} ph_macros ph_system_file_path_checker ph_type_list const_str)
target_link_libraries (${self} PRIVATE ${PROJECT_NAME} ph_macros ph_system_file_path_checker ph_type_list const_str Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${catch_pch} ${boost_pch})
target_compile_definitions (${self} PRIVATE $<$<CONFIG:Debug>:Debug> $<$<CONFIG:Release>:Release> $<$<CONFIG:Kuk>:Kuk>)

install(TARGETS ${self}
    CONFIGURATIONS Release
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

ph_run ()
