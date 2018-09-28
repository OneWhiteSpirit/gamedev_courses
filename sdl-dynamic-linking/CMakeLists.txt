cmake_minimum_required(VERSION 3.9)

project(sdl-dynamic-linking)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sdl_for_builds/include)

find_library(SDL2_LIB libSDL2-2.0.so ${CMAKE_CURRENT_SOURCE_DIR}/../sdl_for_builds/libs)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)
