cmake_minimum_required(VERSION 3.9)
project(hello_libs)

# select SHARED or STATIC library
add_library(hello_libs STATIC include/hello_libs.hpp src/hello_libs.cpp)
target_include_directories(hello_libs PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello_libs PUBLIC cxx_std_17)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /std:c++17")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif()

install(TARGETS hello_libs
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build/
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build/
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build/)
