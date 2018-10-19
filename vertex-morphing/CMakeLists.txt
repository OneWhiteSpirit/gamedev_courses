cmake_minimum_required(VERSION 3.9)

project(vertex-morphing)

find_library(SDL2_LIB NAMES SDL2)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} "src/main.cpp" "src/graphics/window.cpp"
                               "src/graphics/render.cpp" "include/graphics/window.hpp"
                               "include/graphics/render.hpp" "include/math_2d/math_2d.hpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/graphics
                                                  ${CMAKE_SOURCE_DIR}/include/math_2d)

if (MINGW)
    target_link_libraries(${PROJECT_NAME}
               -lmingw32
               -lSDL2main
               -lSDL2
               -mwindows
               -lopengl32
               )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
               -lSDL2
               -lGL
               -lGLEW
               )
endif()


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)

