cmake_minimum_required(VERSION 3.12)

# Project
project(raytracer LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Executable
add_executable(raytracer
    main.cpp  # change if your file has a different name
)

# Warnings
if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /permissive-)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic)
endif()
