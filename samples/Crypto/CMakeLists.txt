cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(CryptoSample)

include_directories(
    ./src
)
    
add_executable(CryptoSample WIN32
    src/CryptoSample.cpp
)

target_link_libraries(CryptoSample 
    libonut
)
