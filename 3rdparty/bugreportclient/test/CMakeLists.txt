cmake_minimum_required(VERSION 3.12)

project(bugreport-test)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

set(breakpad_path ${CMAKE_SOURCE_DIR}/breakpad-bin)

if(WIN32)
    include(${breakpad_path}/lib/windows/client.cmake)
    set(breakpad_client_target_lib ${breakpad_client_lib})
else()
    link_directories(${breakpad_path}/lib/linux/imx6/arm64-4.9.4)
    set(breakpad_client_target_lib breakpad_client_linux pthread)
endif()

find_package(Qt5 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)

add_executable(${PROJECT_NAME} WIN32 
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    bugreport::lib
    ${breakpad_client_target_lib}
)

#设置输出路径
set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
#设置输出文件全路径
set(OUTPUT_FILE_PATH ${INSTALL_DIR}/${PROJECT_NAME})

if (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo") 
    if(WIN32)
    #获取windeployqt.exe的绝对路径
    get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
    set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    message("windeployqt executable path: ${QT5_WINDEPLOYQT_EXECUTABLE}")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        #创建输出文件夹
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
        #拷贝应用程序到输出文件夹
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
        #导出sym文件
        COMMAND ${breakpad_path}/analyze_tool/dump_syms.exe ${OUTPUT_FILE_PATH}.pdb > ${OUTPUT_FILE_PATH}.sym
        #执行windeployqt导出依赖库
        COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --no-translations ${INSTALL_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
    )
    else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        #创建输出文件夹
        COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
        #拷贝应用程序到输出文件夹
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
        #导出sym文件
        COMMAND ${breakpad_path}/analyze_tool/dump_syms ${OUTPUT_FILE_PATH} > ${OUTPUT_FILE_PATH}.sym
        #删除应用调试信息
        COMMAND ${COMPILE_ROOT_PATH}/bin/arm-linux-gnueabihf-objcopy --strip-debug ${OUTPUT_FILE_PATH}
    )
    endif()
endif()