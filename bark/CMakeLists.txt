cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(bark)

file(GLOB src_files ./src/*.*)
file(GLOB src_common ./common/*.*)

source_group("src" FILES ${src_files})
source_group("common" FILES ${src_common})

add_executable(${PROJECT_NAME} WIN32 ${src_files} ${src_common})

if (MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /bigobj")
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

# box2D
option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
add_subdirectory(./box2d/)

target_link_libraries(${PROJECT_NAME} libonut box2d)
target_include_directories(${PROJECT_NAME} PUBLIC ../src/ ./src/ ./common/ box2d/include/)
