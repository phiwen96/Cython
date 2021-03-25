

file(GLOB_RECURSE HEADER_LIST "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB SOURCE_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

set (self ${PROJECT_NAME})

add_library (${self} SHARED
	${SOURCE_LIST}  ${HEADER_LIST} 
)
set_target_properties(${self} PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}.hpp)




ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${boost_pch})

# add_library (ph::type_list ALIAS ph_type_list)
target_link_libraries (${self} PRIVATE ${Boost_LIBRARIES} ph_type_list ph_macros const_str)
target_include_directories (${self} PRIVATE ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS} ph_type_list ph_macros const_str)

target_compile_features (${self} PUBLIC cxx_std_20)


target_include_directories(${self} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${self}>
    $<INSTALL_INTERFACE:include/${self}>
)

target_include_directories(${self} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


include(GNUInstallDirs)
set (CMAKE_BUILD_WITH_INSTALL_RPATH YES)
set (CMAKE_BUILD_WITH_INSTALL_NAME_DIR YES)


install(TARGETS ${self}
    # CONFIGURATIONS Release
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        # COMPONENT ${PROJECT_NAME}_Runtime
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # COMPONENT ${PROJECT_NAME}_Runtime

    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        # COMPONENT ${PROJECT_NAME}_Development
        # CONFIGURATIONS Debug Release

    PUBLIC_HEADER 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

    

ph_run ()