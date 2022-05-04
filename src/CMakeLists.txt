cmake_minimum_required (VERSION 3.8)

project ("bangumireader" VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(PROJECT_UI_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ui")
add_compile_definitions(APP_VERSION="v${PROJECT_VERSION}")
message("current project version: v${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Gui Widgets Quick Network REQUIRED)

set(qnetworkservice ${CMAKE_SOURCE_DIR}/3rdparty/qtnetworkservice/src)
set(filewatcher ${CMAKE_SOURCE_DIR}/3rdparty/qtmintools/filewatcher)

include_directories(${qnetworkservice})

include(${CMAKE_CURRENT_SOURCE_DIR}/entity/entity.cmake)

add_executable (${PROJECT_NAME} WIN32

    main.cpp    
    resource.qrc

    models/bangumilistmodel.h
    models/bangumilistmodel.cpp
    models/sourcelinkmodel.h
    models/sourcelinkmodel.cpp
    models/rsssubscribemodel.h
    models/rsssubscribemodel.cpp
    models/bangumidatabaselistmodel.h
    models/bangumidatabaselistmodel.cpp
    models/bangumidetailmodel.h
    models/bangumidetailmodel.cpp

    utils/mikanrssreader.h
    utils/mikanrssreader.cpp
    utils/bangumidatabasereader.h
    utils/bangumidatabasereader.cpp
    utils/versionchecker.h
    utils/versionchecker.cpp

    ${filewatcher}/filewatcher.h
    ${filewatcher}/filewatcher.cpp

    ${qnetworkservice}/HttpClient.h

    ${ENTITY_FILE_LIST}
    entity/sqliteconfig.h

    databasemodels/settingtbmodel.h
    databasemodels/settingtbmodel.cpp
    databasemodels/remarktbmodel.h
    databasemodels/remarktbmodel.cpp
    databasemodels/subscribemodel.h
    databasemodels/subscribemodel.cpp
    databasemodels/bangumidatamodel.h
    databasemodels/bangumidatamodel.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    Qt5::Network
    qtdao::lib
)