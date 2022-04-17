cmake_minimum_required (VERSION 3.8)

project ("mikanbangumireader")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qtwidgetex/QtWidgetEx)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qtwidgetex/QtWidgetExPlugin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qrouter/src)

set(qfonticonsrc ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qfonticon/QFontIcon)
set(shadowwidgetsrc ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/shadowwidget)
set(qnetworkservice ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qtnetworkservice/src)
set(smoothscrollbar ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smoothscrollbar)

include_directories(${qfonticonsrc} ${shadowwidgetsrc} ${qnetworkservice} ${smoothscrollbar})

add_executable (${PROJECT_NAME} WIN32

    src/main.cpp    
    src/resource.qrc

    src/mikanbangumireader.cpp
    src/mikanbangumireader.h

    src/pages/bangumilistpage.h
    src/pages/bangumilistpage.cpp
    src/pages/bangumilistpage.ui

    #3rdparty
    ${qfonticonsrc}/qfonticon.h
    ${qfonticonsrc}/qfonticon.cpp

    ${shadowwidgetsrc}/ShadowWidget.h
    ${shadowwidgetsrc}/ShadowWidget.cpp

    ${qnetworkservice}/HttpClient.h

    ${smoothscrollbar}/SmoothScrollbar.h
    ${smoothscrollbar}/SmoothScrollbar.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Network
    qtwidgetex::lib
    qrouter::lib
)