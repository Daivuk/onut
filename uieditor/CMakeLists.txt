cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(uieditor)
    
add_executable(uieditor WIN32
    src/main.cpp
    src/DocumentView.cpp
    src/events.cpp
    src/menu.cpp
    src/styles.cpp
    src/viewStyles.cpp
    src/DocumentView.h
    src/events.h
    src/menu.h
    src/styles.h
    src/viewStyles.h
)

if (MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /bigobj")
endif()

target_link_libraries(uieditor 
    libonut
)
