cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(EffectsSample)

include_directories(
    ./src
)
    
add_executable(EffectsSample WIN32
    src/EffectsSample.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(EffectsSample 
    libonut
)
