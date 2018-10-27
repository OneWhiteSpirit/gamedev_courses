cmake_minimum_required(VERSION 3.9)

project(engine)

find_library(SDL2_LIB NAMES SDL2)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SOME_PARAM "src/*.cpp" "src/graphics/*.cpp" "game/src/*.cpp")

add_executable(${PROJECT_NAME} "main.cpp" "game.cpp" ${SOME_PARAM})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/
                                                  ${CMAKE_SOURCE_DIR}/include/graphics
                                                  ${CMAKE_SOURCE_DIR}/game/include
                                                  )


if (MINGW)
    target_link_libraries(${PROJECT_NAME}
               -lmingw32
               -lSDL2main
               -lSDL2
               -mwindows
               -lopengl32               
               -lGL
               -lGLEW
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

