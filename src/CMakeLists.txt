cmake_minimum_required (VERSION 3.8)

project ("mikanbangumireader")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(PROJECT_UI_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt5 COMPONENTS Core Gui Widgets Quick Network REQUIRED)

set(qnetworkservice ${CMAKE_SOURCE_DIR}/3rdparty/qtnetworkservice/src)
set(filewatcher ${CMAKE_SOURCE_DIR}/3rdparty/qtmintools/filewatcher)

include_directories(${qnetworkservice})

add_executable (${PROJECT_NAME} WIN32

    main.cpp    
    resource.qrc

    models/bangumilistmodel.h
    models/bangumilistmodel.cpp

    ${filewatcher}/filewatcher.h
    ${filewatcher}/filewatcher.cpp

    ${qnetworkservice}/HttpClient.h
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    Qt5::Network
)