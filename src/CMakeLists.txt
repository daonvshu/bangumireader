cmake_minimum_required (VERSION 3.8)

project ("mikanbangumireader")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_definitions(-DCONFIG_NETWORK_IMG)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

set(qfonticonsrc ${CMAKE_SOURCE_DIR}/3rdparty/qfonticon/QFontIcon)
set(shadowwidgetsrc ${CMAKE_SOURCE_DIR}/3rdparty/shadowwidget)
set(qnetworkservice ${CMAKE_SOURCE_DIR}/3rdparty/qtnetworkservice/src)
set(smoothscrollbar ${CMAKE_SOURCE_DIR}/3rdparty/smoothscrollbar)

include_directories(${qfonticonsrc} ${shadowwidgetsrc} ${qnetworkservice} ${smoothscrollbar})

add_executable (${PROJECT_NAME} WIN32

    main.cpp    
    resource.qrc

    mikanbangumireader.cpp
    mikanbangumireader.h

    pages/bangumilistpage.h
    pages/bangumilistpage.cpp
    pages/bangumilistpage.ui
    pages/bangumilistitem.ui

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