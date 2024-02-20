# Add source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create executable target
add_executable(Sandbox ${SOURCE_FILES})

# Add include directory
target_include_directories(Sandbox
PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/pch"
INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

# Link the static library
target_link_libraries(Sandbox
PRIVATE
	Patchouli
)

# Define configuration-specific macros
define_config_macros(Sandbox)
