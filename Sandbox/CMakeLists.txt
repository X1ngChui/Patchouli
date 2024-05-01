# Add source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create executable target
add_executable(Sandbox ${SOURCE_FILES})

# Custom target for resources
add_custom_target(UpdateResources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Sandbox>/assets
    COMMENT "Updating resources..."
)

# Make Sandbox depend on UpdateResources
add_dependencies(Sandbox UpdateResources)

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

# Platform-specific settings
if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
	set_target_properties(Sandbox PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()