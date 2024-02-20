# Define a CMake function for importing and configuring third-party libraries
function(add_third_party LIBRARY_NAME)
    # Set the source code path for the library
    set(LIBRARY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/third-party/${LIBRARY_NAME}")

    # Add the CMakeLists.txt file for the library
    add_subdirectory(${LIBRARY_SOURCE_DIR} EXCLUDE_FROM_ALL)
endfunction()
