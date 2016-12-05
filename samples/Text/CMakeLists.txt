cmake_minimum_required(VERSION 3.0)

project(TextSample)


include_directories(
    ./src
)

add_executable(TextSample
    src/TestSample.cpp
)

target_link_libraries(TextSample 
    onut
)
