cmake_minimum_required(VERSION 3.9)
project(draw_and_guess_game)

set(CMAKE_CXX_STANDARD 11)

include_directories(./lib/ZMQ)

add_executable(server ./include/util.h ./include/server_publisher.h ./src/server/server_publisher.cpp)
