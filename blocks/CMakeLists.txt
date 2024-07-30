cmake_minimum_required(VERSION 3.10)
project(Blocks)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(src)
include_directories(src/platform)
include_directories(src/external)
include_directories(src/resource_loader)

# Specify source files
set(SOURCES
    src/camera.c
    src/linear_algebra.c
    src/main.c
    src/utils.c
    src/resource_loader/argb_image_loader.c
)

# Conditionally add platform-specific files
if(WIN64)
    message(STATUS "Configuring for Windows")
    set(SOURCES ${SOURCES} src/platform/Windows/platform_win64.c)
elseif(UNIX)
    message(STATUS "Configuring for Unix/Linux")
    set(SOURCES ${SOURCES} src/platform/Linux/platform_linux.c)
endif()

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
