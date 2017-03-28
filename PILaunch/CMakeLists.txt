cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if (UNIX AND NOT APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)
elseif (APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/osx)
elseif (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/win)
endif()

project(PILaunch)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=3")
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
endif()

add_executable(PILaunch
    main.cpp
)
