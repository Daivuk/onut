cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/linux)

project(JSStandAlone)
    
add_executable(JSStandAlone
    src/main.cpp
)

target_link_libraries(JSStandAlone 
    onut
)
