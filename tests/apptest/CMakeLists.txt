# Testing app
add_executable(testapp main.cpp
    # COMMAND 
)

file (GLOB PRE ${CMAKE_CURRENT_LIST_DIR}/sources/*)
# file (GLOB POST $)

foreach (pre ${PRE})
    message (${pre})
    get_filename_component (ext ${pre} EXT)

    if (${ext} STREQUAL ".in.hpp")
        get_filename_component(name ${pre} NAME_WE)
        add_custom_command(
                TARGET testapp
                PRE_BUILD
                # OUTPUT ${CMAKE_CURRENT_LIST_DIR}/sources/${name}.cpp
                COMMAND $<TARGET_FILE:cython_app> --input ${pre} --output ${CMAKE_CURRENT_LIST_DIR}/sources/${name}.hpp
                DEPENDS ${pre}
                VERBATIM
    )
    endif ()
    
    # target_sources (testapp PRIVATE ${pre})
endforeach ()
target_sources (testapp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/sources)

# target_sources (testapp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/sources)

# foreach(scm_source ${SCM_SOURCES})
#     get_filename_component(file_c ${scm_source} NAME_WE)
#     set(file_c "${file_c}.c")
    # add_custom_command(
    #         OUTPUT ${file_c}
    #         COMMAND gsc -c ${CMAKE_CURRENT_SOURCE_DIR}/${scm_source}
    #         DEPENDS ${scm_source}
    #         VERBATIM
    # )
#     list(APPEND scm_c_files ${file_c})
# endforeach()