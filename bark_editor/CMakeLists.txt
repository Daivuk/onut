cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(bark_editor)

file(GLOB src_files ./src/*.*)
add_executable(${PROJECT_NAME} WIN32 ${src_files})

if (MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /bigobj")
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(${PROJECT_NAME} libonut)
target_compile_definitions(${PROJECT_NAME} PRIVATE BARK_EDITOR=1)
