cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glad
    ${CMAKE_SOURCE_DIR}/include/GLFW
    ${CMAKE_SOURCE_DIR}/include/KHR
)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)
list(APPEND SOURCES
    main.cpp
    include/glad/glad.c
)

# Create executable
add_executable(OpenGL ${SOURCES})

# Link libraries
target_link_libraries(OpenGL
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    OpenGL::GL
    assimp::assimp
    Jolt::Jolt
)

# Copy shader files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

# macOS specific: link Cocoa, IOKit, and CoreVideo frameworks
if(APPLE)
    target_link_libraries(OpenGL
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

