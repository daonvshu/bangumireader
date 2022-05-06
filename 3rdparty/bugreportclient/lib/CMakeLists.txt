cmake_minimum_required(VERSION 3.12)

project(bugreport-lib)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

if (WIN32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../breakpad-bin/includes/windows)
add_compile_definitions(BUGREPORT_EXECUTABLE="bugreport.exe")
else()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../breakpad-bin/includes/linux)
add_compile_definitions(BUGREPORT_EXECUTABLE="./bugreport")
endif()

add_library(${PROJECT_NAME} 
    log.h
    log.cpp

    crashlistener.h
    crashlistener.cpp
)

add_library(bugreport::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Network
)