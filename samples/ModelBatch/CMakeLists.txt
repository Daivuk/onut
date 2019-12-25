cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(ModelBatchSample)

include_directories(
    ./src
)
    
add_executable(ModelBatchSample WIN32
    src/ModelBatchSample.cpp
)

target_link_libraries(ModelBatchSample 
    libonut
)
