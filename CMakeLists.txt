# Minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Project name
project("Patchouli")

# Build options
option(PATCHOULI_LIBRARY_TYPE "Build shared library instead of static library" ON)

# Set language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directories for executable, library, and symbol files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/dist/${CMAKE_BUILD_TYPE})

# Compiler-specific settings
if(MSVC)
    # MSVC compiler settings
    # Debug mode settings (with pdb, no optimization)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    
    # Testing mode settings (with pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_TESTING "${CMAKE_CXX_FLAGS_TESTING} /Ox /Zi")
    set(CMAKE_C_FLAGS_TESTING "${CMAKE_C_FLAGS_TESTING} /Ox /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_TESTING "${CMAKE_EXE_LINKER_FLAGS_TESTING} /INCREMENTAL:NO /OPT:REF /OPT:ICF /DEBUG")

    # Release mode settings (no pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO /OPT:REF /OPT:ICF")
else()
    # GNU or Clang compiler settings
    # Debug mode settings (with gdb, no optimization)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

    # Testing mode settings (with gdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_TESTING "${CMAKE_CXX_FLAGS_TESTING} -Ofast -g")
    set(CMAKE_C_FLAGS_TESTING "${CMAKE_C_FLAGS_TESTING} -Ofast -g")

    # Release mode settings (no pdb, optimization enabled)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast")
endif()

# Include custom fuctions
include(cmakes/ThirdParty.cmake)
include(cmakes/ConfigMacros.cmake)

# Add subdirectory
add_subdirectory(Patchouli)
add_subdirectory(Sandbox)

# Add third-party library
add_third_party(mimalloc)
add_third_party(fmt)
add_third_party(spdlog)
add_third_party(glfw)
