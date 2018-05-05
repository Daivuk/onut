cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(Imgui)

include_directories(
    ./src
)
    
add_executable(ImguiSample WIN32
    src/ImguiSample.cpp
)

target_link_libraries(ImguiSample 
    libonut
)
