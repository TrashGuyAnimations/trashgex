cmake_minimum_required(VERSION 3.23)
project(trashgex)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE pcre2-8)