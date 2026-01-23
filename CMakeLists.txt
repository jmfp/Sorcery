cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add Assimp using FetchContent
include(FetchContent)
set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.3.1
)
FetchContent_MakeAvailable(assimp)

# Add Jolt Physics
FetchContent_Declare(
    JoltPhysics
    GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
    GIT_TAG v5.5.0
)

set(JPH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(JoltPhysics)

# Add ImGui
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(ImGui)

# Get ImGui source directory
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_SRC_DIR)
if(NOT IMGUI_SRC_DIR)
    set(IMGUI_SRC_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src")
endif()

# Create ImGui library target
add_library(imgui STATIC
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_demo.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SRC_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC 
    ${IMGUI_SRC_DIR}
    ${IMGUI_SRC_DIR}/backends
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/GLFW
)

# Build Jolt Physics library
# After FetchContent_MakeAvailable, the source is at build/_deps/<name>-src
# We can construct the path or use the variable that FetchContent sets
# The standard variable name is <name>_SOURCE_DIR, but FetchContent_GetProperties ensures it's set
FetchContent_GetProperties(JoltPhysics SOURCE_DIR JOLT_SRC_DIR BINARY_DIR JOLT_BIN_DIR)

# Fallback: construct path if variable not set (shouldn't happen, but just in case)
if(NOT JOLT_SRC_DIR)
    set(JOLT_SRC_DIR "${CMAKE_BINARY_DIR}/_deps/joltphysics-src")
endif()

if(NOT EXISTS "${JOLT_SRC_DIR}/Jolt/Jolt.cmake")
    message(FATAL_ERROR "JoltPhysics Jolt.cmake not found at ${JOLT_SRC_DIR}/Jolt/Jolt.cmake")
endif()

# Set PHYSICS_REPO_ROOT that Jolt.cmake expects, then include it directly
set(PHYSICS_REPO_ROOT ${JOLT_SRC_DIR})
include(${JOLT_SRC_DIR}/Jolt/Jolt.cmake)

# Engine source files (excluding entry points)
file(GLOB_RECURSE ENGINE_SOURCES
    "src/*.cpp"
)

# Create object library for shared engine code
add_library(engine_objects OBJECT
    ${ENGINE_SOURCES}
    include/glad/glad.c
)

# Set include directories for engine objects
target_include_directories(engine_objects PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glad
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/KHR
)

# Link common dependencies to engine objects
target_link_libraries(engine_objects PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    OpenGL::GL
    assimp::assimp
    Jolt::Jolt
)

# macOS frameworks for engine objects
if(APPLE)
    target_link_libraries(engine_objects PUBLIC
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# Engine executable (main.cpp)
add_executable(OpenGL
    main.cpp
    $<TARGET_OBJECTS:engine_objects>
)

target_link_libraries(OpenGL PRIVATE engine_objects)

# Editor executable (editor/editor.cpp)
add_executable(Editor
    editor/editor.cpp
    $<TARGET_OBJECTS:engine_objects>
)

target_link_libraries(Editor PRIVATE 
    engine_objects
    imgui
)

# macOS frameworks for Editor
if(APPLE)
    target_link_libraries(Editor PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# Copy shader files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})


