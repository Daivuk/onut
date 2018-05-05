cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(GamePadsSample)

include_directories(
    ./src
)
    
add_executable(GamePadsSample WIN32
    src/GamePadsSample.cpp
)

target_link_libraries(GamePadsSample 
    libonut
)
