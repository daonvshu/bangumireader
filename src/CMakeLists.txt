cmake_minimum_required (VERSION 3.8)

project ("bangumireader" VERSION 1.0.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(PROJECT_UI_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ui")
add_compile_definitions(APP_VERSION="v${PROJECT_VERSION}")
message("current project version: v${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Gui Widgets Quick Network REQUIRED)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    find_package(Qt5QuickCompiler)
    qtquick_compiler_add_resources(UI_RESOURCES ui.qrc)
endif()

set(qnetworkservice ${CMAKE_SOURCE_DIR}/3rdparty/qtnetworkservice/src)
set(filewatcher ${CMAKE_SOURCE_DIR}/3rdparty/qtmintools/filewatcher)

#设置breakpad库路径
set(breakpad_path ${CMAKE_SOURCE_DIR}/3rdparty/bugreportclient/breakpad-bin)
#设置breakpad库路径
if(WIN32)
    include(${breakpad_path}/lib/windows/client.cmake)
    set(breakpad_client_target_lib ${breakpad_client_lib})
endif()

include_directories(${qnetworkservice})

include(${CMAKE_CURRENT_SOURCE_DIR}/entity/entity.cmake)

add_executable (${PROJECT_NAME} WIN32

    main.cpp    
    resource.qrc
    resource.rc

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
    utils/myquickview.h
    utils/myquickview.cpp
    utils/rssnotifyview.h
    utils/rssnotifyview.cpp

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

    ${UI_RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    Qt5::Network
    qtdao::lib

    #添加bugreport库
    bugreport::lib
    #添加breakpad库
    ${breakpad_client_target_lib}
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    #获取windeployqt.exe的绝对路径
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    message("windeployqt executable path: ${QT5_WINDEPLOYQT_EXECUTABLE}")

    set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/resource.rc)

    message("install dir: ${INSTALL_DIR}")

    file(GLOB OpenSslRuntime ${CMAKE_SOURCE_DIR}/openssl/*.dll)
    message("openssl runtime: ${OpenSslRuntime}")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        #创建输出文件夹
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
        #拷贝应用程序到输出文件夹
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
        #执行windeployqt导出依赖库
        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --no-translations ${INSTALL_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}> -qmldir ${CMAKE_CURRENT_SOURCE_DIR}/ui
        #拷贝符号文件
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
        #导出sym文件
        COMMAND ${breakpad_path}/analyze_tool/dump_syms.exe ${INSTALL_DIR}/${PROJECT_NAME}.pdb > ${INSTALL_DIR}/${PROJECT_NAME}.sym
        #复制openssl
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${OpenSslRuntime} ${INSTALL_DIR}
    )
endif()