cmake_minimum_required(VERSION 3.5)

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

# Work dir
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

if (MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" "/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1 /bigobj")
endif()

target_link_libraries(uieditor 
    libonut
)
