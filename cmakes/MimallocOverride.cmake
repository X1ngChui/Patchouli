function(mimalloc_override TARGET_NAME)
    if (WIN32)
        get_target_property(type ${TARGET_NAME} TYPE)
        if (type STREQUAL "EXECUTABLE" OR type STREQUAL "SHARED_LIBRARY")
            add_custom_command(
                TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND ${PROJECT_SOURCE_DIR}/third-party/mimalloc/bin/minject -f -i $<TARGET_FILE:${TARGET_NAME}>
                COMMENT "Inplace shared libraries for ${TARGET_NAME}..."
            )
        endif()
    endif()
endfunction()