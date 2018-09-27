cmake_minimum_required(VERSION 3.9)
project(hello_bins)

add_executable(hello_bins src/main.cpp)
target_compile_features(hello_bins PRIVATE cxx_std_17)
target_link_libraries(hello_bins LINK_PUBLIC hello_libs)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /std:c++17")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
endif()

install(TARGETS hello_bins
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../build)
