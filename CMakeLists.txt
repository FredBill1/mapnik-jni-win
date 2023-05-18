cmake_minimum_required(VERSION 3.15)

project(mapnik-jni)

set(CMAKE_CXX_STANDARD 11)

find_package(JNI REQUIRED)
find_package(mapnik CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Release)
mapnik_find_plugin_dir(MAPNIK_PLUGINS_DIR)
message(STATUS "MAPNIK_PLUGINS_DIR: ${MAPNIK_PLUGINS_DIR}")
add_definitions(
    -DMAPNIK_INPUT_PLUGINS_DIR="${MAPNIK_PLUGINS_DIR}"
)

add_library(${PROJECT_NAME} SHARED
    csrc/mapnikjni.cpp
    csrc/mapnikjni.h
)

message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${JNI_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    mapnik::mapnik mapnik::json mapnik::wkt
)