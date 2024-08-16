cmake_minimum_required(VERSION 3.10)
project(Blocks)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(src)
include_directories(src/external)
include_directories(src/resource_loader)

# Specify source files
set(SOURCES
    src/platform.c
    src/main.c
    src/world.c
    src/camera.c
    src/utils.c
    src/chunk.c
    src/render.c
    src/resource_manager/argb_image_loader.c
    src/resource_manager/resource_manager.c
)

# Add the executable
add_executable(Blocks ${SOURCES})

# Link necessary libraries
if(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(Blocks X11 pthread m) # Added 'm' for math library
endif()

# Specify output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Optionally, specify where to put intermediate binaries
set(CMAKE_INTERMEDIATE_DIR ${CMAKE_BINARY_DIR}/intermediate)
