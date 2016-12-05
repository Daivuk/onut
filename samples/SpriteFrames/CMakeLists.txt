cmake_minimum_required(VERSION 3.0)

project(SpriteFramesSample)

include_directories(
    ./src
)

add_executable(SpriteFramesSample
    src/SpriteFramesSample.cpp
)

target_link_libraries(SpriteFramesSample 
    onut
)
