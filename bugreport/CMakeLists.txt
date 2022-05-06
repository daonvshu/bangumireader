cmake_minimum_required(VERSION 3.12)

project(bugreport)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(${PROJECT_NAME} WIN32 
    main.cpp

    bugreport.h
    bugreport.cpp
    bugreport.ui
    bugreport.qrc
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release") 
#设置输出路径
set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #创建输出文件夹
    COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
    #拷贝应用程序到输出文件夹
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
)
endif()