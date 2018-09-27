cmake_minimum_required(VERSION 3.10)

project(sdl-static-linking)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../sdl_for_builds/include)

find_library(SDL2_LIB libSDL2.a ${CMAKE_CURRENT_SOURCE_DIR}/../sdl_for_builds/lib)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB} -lm -ldl -lpthread)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)
