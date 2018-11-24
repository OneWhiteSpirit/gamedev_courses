cmake_minimum_required(VERSION 3.9)

project(engine)

find_library(SDL2_LIB NAMES SDL2)
find_library(SDL2_MIXER NAMES SDL2_mixer)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

file(GLOB SOME_PARAM "*.cpp" "engine/src/*.cpp" "engine/src/graphics/*.cpp" "engine/src/imgui/*.cpp" "game/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOME_PARAM})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/engine/include/
                                                  ${CMAKE_SOURCE_DIR}/engine/include/graphics                                                  
                                                  ${CMAKE_SOURCE_DIR}/engine/include/imgui
                                                  ${CMAKE_SOURCE_DIR}/game/include)

if (MINGW)
    target_link_libraries(${PROJECT_NAME}
               -lmingw32
               -lSDL2main
               -lSDL2
               -mwindows
               -lopengl32                              
               -lglew32               
               -lSDL2_mixer
               )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME}
               -lSDL2
               -lGL
               -lGLEW
               ${SDL2_MIXER}
               )
endif()


install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin)

