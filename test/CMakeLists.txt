cmake_minimum_required (VERSION 3.8)

project ("test")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp    
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
)