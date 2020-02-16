cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(CustomMeshSample)

include_directories(
    ./src
)
    
add_executable(CustomMeshSample WIN32
    src/CustomMeshSample.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(CustomMeshSample 
    libonut
)
