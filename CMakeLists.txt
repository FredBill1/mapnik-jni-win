cmake_minimum_required(VERSION 3.17)

project(mapnik-jni)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_FIND_DEBUG_MODE FALSE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD.lib")

find_package(JNI REQUIRED)
find_package(mapnik CONFIG REQUIRED)
find_package(GDAL CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc io dt in)
find_package(unofficial-lerc CONFIG REQUIRED)
find_package(GEOS CONFIG REQUIRED)

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)

    # list(SORT _variableNames)
    foreach(_variableName ${_variableNames})
        if(ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})

            if(NOT MATCHED)
                continue()
            endif()
        endif()

        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

dump_cmake_variables()
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
    GDAL::GDAL
    ICU::uc ICU::io ICU::dt ICU::in
    unofficial::Lerc::Lerc
    GEOS::geos_c
)