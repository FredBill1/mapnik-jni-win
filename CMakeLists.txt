cmake_minimum_required(VERSION 3.17)

project(mapnik-jni)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_FIND_DEBUG_MODE FALSE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(MAPNIK_JNI_MSVC_USE_STATIC_LINKING "Use static linking for MSVC" OFF)
message(STATUS "MAPNIK_JNI_MSVC_USE_STATIC_LINKING: ${MAPNIK_JNI_MSVC_USE_STATIC_LINKING}")

if(MSVC AND MAPNIK_JNI_MSVC_USE_STATIC_LINKING)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD.lib")
endif()

add_subdirectory(modules/mapnik-vector-tile)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
find_package(mapnik CONFIG REQUIRED)
find_package(GDAL CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc io dt in)
find_package(unofficial-lerc CONFIG REQUIRED)
find_package(GEOS CONFIG REQUIRED)

include(UseJava)

file(GLOB_RECURSE JAVA_SOURCES src/geowin/mapnik/*.java)
add_jar(${PROJECT_NAME}_jar ${JAVA_SOURCES} GENERATE_NATIVE_HEADERS ${PROJECT_NAME}_native)

file(GLOB_RECURSE JNI_SOURCES csrc/*.cpp)
add_library(${PROJECT_NAME} SHARED ${JNI_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    mapnik::mapnik mapnik::json mapnik::wkt
    GDAL::GDAL
    ICU::uc ICU::io ICU::dt ICU::in
    unofficial::Lerc::Lerc
    GEOS::geos_c
    mapnik-vector-tile
    ${PROJECT_NAME}_native
)
target_include_directories(${PROJECT_NAME} PRIVATE ${JNI_INCLUDE_DIRS})
target_precompile_headers(${PROJECT_NAME} PRIVATE "csrc/globals.hpp")
