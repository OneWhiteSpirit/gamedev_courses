cmake_minimum_required(VERSION 3.9)

project(sdl-dynamic-linking)

set(CMAKE_CXX_STANDARD 17)
#set(SDL2_INCLUDE_DIRS ../sdl_for_builds/include/)
#set(SDL2_LIBPATH "${CMAKE_CURRENT_LIST_DIR}/../sdl_for_builds/libs/")

#find_library(SDL2_LIB SDL2 PATH ${SDL2_LIBPATH})

find_package(SDL2 REQUIRED)

#set(SDL2_LIBRARIES ${SDL2_LIB})

#include(FindPkgConfig)
#pkg_search_module(SDL2 REQUIRED sdl2 >= 2.0.0)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)
