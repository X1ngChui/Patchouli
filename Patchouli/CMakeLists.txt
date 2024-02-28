# Add source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create library target
if (PATCHOULI_LIBRARY_TYPE)
	add_library(Patchouli SHARED ${SOURCE_FILES})
	target_compile_definitions(Patchouli PUBLIC PATCHOULI_SHARED_LIBRARY)
	target_compile_definitions(Patchouli PRIVATE PATCHOULI_SHARED_LIBRARY_EXPORT)
else()
	add_library(Patchouli STATIC ${SOURCE_FILES})
	target_compile_definitions(Patchouli PUBLIC PATCHOULI_STATIC_LIBRARY)
endif()

# Find external package
find_package(Vulkan REQUIRED)
if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found. Please make sure Vulkan SDK is installed.")
endif()

# Add include directory
target_include_directories(Patchouli
PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/pch"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/third-party/mimalloc/include"
	"${CMAKE_SOURCE_DIR}/third-party/fmt/include"
	"${CMAKE_SOURCE_DIR}/third-party/spdlog/include"
	"${CMAKE_SOURCE_DIR}/third-party/glfw/include"
	"${Vulkan_INCLUDE_DIRS}"
INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/pch"
	"${CMAKE_SOURCE_DIR}/third-party/fmt/include"
	"${CMAKE_SOURCE_DIR}/third-party/spdlog/include"
)

# Link the static library
target_link_libraries(Patchouli
PRIVATE
	${Vulkan_LIBRARIES}
	mimalloc-static
	glfw
PUBLIC
	spdlog_header_only
	fmt-header-only
)

# Specify the precompiled header file
target_precompile_headers(Patchouli PRIVATE "pch/PatchouliPch.h")

# Define configuration-specific macros
define_config_macros(Patchouli)
if (PATCHOULI_CONSOLE_ENABLE)
	target_compile_definitions(Patchouli PUBLIC PATCHOULI_CONSOLE_ENABLE)
endif()
