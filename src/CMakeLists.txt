

file(GLOB_RECURSE HEADER_LIST "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB SOURCE_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

set (self ${PROJECT_NAME}_lib)

add_library (${self}
	${SOURCE_LIST}  ${HEADER_LIST} 
)





ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${boost_pch})

# add_library (ph::type_list ALIAS ph_type_list)
target_link_libraries (${self} PRIVATE ${Boost_LIBRARIES} ph_type_list const_str)
target_include_directories (${self} PRIVATE ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ph_type_list const_str)

target_compile_features (${self} PUBLIC cxx_std_20)


target_include_directories(${self} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${self}>
    $<INSTALL_INTERFACE:include/${self}>
)

target_include_directories(${self} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)