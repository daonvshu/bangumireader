## BugReport错误信息收集库

### cmake配置
```cmake
#...
project(xxxx)
#...
#设置breakpad库路径
set(breakpad_path ${CMAKE_SOURCE_DIR}/3rdparty/bugreportclient/breakpad-bin)
#设置breakpad库路径
if(WIN32)
    include(${breakpad_path}/lib/windows/client.cmake)
    set(breakpad_client_target_lib ${breakpad_client_lib})
else()
    link_directories(${breakpad_path}/lib/linux/imx6/arm64-4.9.4)
    set(breakpad_client_target_lib breakpad_client_linux pthread)
endif()
#...
find_package(...)
#...
target_link_libraries(${PROJECT_NAME}
    #...
    #添加bugreport库
    bugreport::lib
    #添加breakpad库
    ${breakpad_client_target_lib}
)
#...
#设置输出路径
set(INSTALL_DIR ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
#设置输出文件全路径
set(OUTPUT_FILE_PATH ${INSTALL_DIR}/${PROJECT_NAME})

if (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo") 
    if(WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            #创建输出文件夹
            COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_DIR}
            #拷贝应用程序到输出文件夹
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PDB_FILE:${PROJECT_NAME}> ${INSTALL_DIR}
            #导出sym文件
            COMMAND ${breakpad_path}/analyze_tool/dump_syms.exe ${OUTPUT_FILE_PATH}.pdb > ${OUTPUT_FILE_PATH}.sym
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
```

### 代码中使用该库
```cpp
#include "crashlistener.h"
#include "log.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    //日志输出到控制台
    Log::useQDebugOnly();

    //设置应用accessKey
    CrashListener crashListener(
        "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJBdXRoZW50aWNhdGlvbiIsImlzcyI"
        "6Ind3dy5lYW50dGVjaC5jb20iLCJuYW1lIjoiYnVncmVwb3J0LXRlc3QifQ.V4HgOtImEXVWYdS_Y6TStap-xBdeFwIciR86X29kYYU"
    );

    return a.exec();
}
```
> linux

CMakeSettings.json中修改configtype为 `RelWithDebInfo`
```json
{
    "name": "WSL-GCC-Release",
    "configurationType": "RelWithDebInfo"
}
```