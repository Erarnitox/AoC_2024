cmake_minimum_required(VERSION 3.30)

project(AoC LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "*.cpp")

foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(FILENAME ${SOURCE_FILE} NAME_WE)
    add_executable(${FILENAME} ${SOURCE_FILE})
endforeach()
