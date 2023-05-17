cmake_minimum_required(VERSION 3.15)

project(mapnik-jni)

set(CMAKE_CXX_STANDARD 11)

find_package(JNI REQUIRED)
find_package(mapnik CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED
    csrc/mapnikjni.cpp
    csrc/mapnikjni.h
)

message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${JNI_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    mapnik::mapnik
)