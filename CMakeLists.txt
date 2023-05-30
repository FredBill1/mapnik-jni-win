cmake_minimum_required(VERSION 3.17)

project(mapnik-jni)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_FIND_DEBUG_MODE FALSE)

if(0)
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

file(GLOB JAVA_SOURCES src/mapnik/*.java)
add_jar(${PROJECT_NAME}_jar ${JAVA_SOURCES} GENERATE_NATIVE_HEADERS ${PROJECT_NAME}_native)

file(GLOB JNI_SOURCES csrc/*.cpp)
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
