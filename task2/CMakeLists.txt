cmake_minimum_required(VERSION 3.5)

project(task2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(task2 main.cpp)

install(TARGETS task2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(task2 ${OpenCV_LIBS} )
