cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ShaderSample)

include_directories(
    ./src
)
    
add_executable(ShaderSample WIN32
    src/ShaderSample.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(ShaderSample 
    libonut
)
