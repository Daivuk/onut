cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(CursorSample)

include_directories(
    ./src
)
    
add_executable(CursorSample WIN32
    src/CursorSample.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(CursorSample 
    libonut
)
