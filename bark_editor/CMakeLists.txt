cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(bark_editor)

file(GLOB src_files ./src/*.*)
file(GLOB src_panels ./src/panels/*.*)
file(GLOB src_common ../bark/common/*.*)

source_group("src" FILES ${src_files})
source_group("panels" FILES ${src_panels})
source_group("common" FILES ${src_common})

add_executable(${PROJECT_NAME} WIN32 ${src_files} ${src_panels} ${src_common})

if (MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /bigobj")
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(${PROJECT_NAME} libonut)
target_compile_definitions(${PROJECT_NAME} PRIVATE BARK_EDITOR=1)
target_include_directories(${PROJECT_NAME} PUBLIC ../bark/common/ ./src/ ./src/panels/)
