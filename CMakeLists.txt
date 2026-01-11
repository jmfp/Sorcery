cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

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

