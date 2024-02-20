# Define a CMake function to set configuration-specific macros
function(define_config_macros target_name)
    string(TOUPPER ${target_name} PROJECT_NAME_UPPERCASE)
    # Define XXXX_DEBUG_MODE macro in Debug configuration
    target_compile_definitions(${target_name} PRIVATE $<$<CONFIG:Debug>:${PROJECT_NAME_UPPERCASE}_DEBUG_MODE>)
    # Define XXXX_TESTING_MODE macro in Testing configuration
    target_compile_definitions(${target_name} PRIVATE $<$<CONFIG:Testing>:${PROJECT_NAME_UPPERCASE}_TESTING_MODE>)
    # Define XXXX_RELEASE_MODE macro in Release configuration
    target_compile_definitions(${target_name} PRIVATE $<$<CONFIG:Release>:${PROJECT_NAME_UPPERCASE}_RELEASE_MODE>)
endfunction()