cmake_minimum_required(VERSION 3.9)

project(sdl-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "src/main.cpp" "src/engine.cpp" "include/engine.hpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)

