cmake_minimum_required(VERSION 3.12)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release") 
    set(OUTPUT_TYPE_SUFFIX "d")
endif()

#x86/x64
if (CMAKE_CL_64)
    set(OUTPUT_CL_SUFFIX "_x64")
else()
    set(OUTPUT_CL_SUFFIX "_x86")
endif()

set(breakpad_client_lib breakpad_client${OUTPUT_TYPE_SUFFIX}${OUTPUT_CL_SUFFIX}.lib)

link_directories(${CMAKE_CURRENT_LIST_DIR})

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../includes/windows)