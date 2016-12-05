cmake_minimum_required(VERSION 3.0)

project(SpritesSample)

include_directories(
    ./src
)
    
add_executable(SpritesSample
    src/SpritesSample.cpp
)

target_link_libraries(SpritesSample 
    onut
)
