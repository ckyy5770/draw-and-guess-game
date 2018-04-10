cmake_minimum_required(VERSION 3.9)
project(draw_and_guess_game)

set(CMAKE_CXX_STANDARD 11)


## use this to globally use C++11 with in our project
set(CMAKE_CXX_STANDARD 11)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

## pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



include_directories(./lib/ZMQ)

add_executable(server main/server_main.cpp src/server/server_publisher.cpp include/util.h include/server_publisher.h include/server_request_responder.h include/server_drawing_listener.h include/server_player_list.h include/game_player.h include/server_game_list.h include/server_game_room.h include/server.h)

add_executable(client main/client_main.cpp include/util.h include/client_drawing_sender.h include/client_request_sender.h include/client_subscriber.h include/game_player.h include/client_game_room.h include/client.h include/client_game_list.h)



## add the include directory to our compile directives
target_include_directories(server PUBLIC ${ZeroMQ_INCLUDE_DIR})
## at the 0mq, pthread library to our link directive
target_link_libraries(server PUBLIC ${ZeroMQ_LIBRARY} Threads::Threads)

## add the include directory to our compile directives
target_include_directories(client PUBLIC ${ZeroMQ_INCLUDE_DIR})
## at the 0mq, pthread library to our link directive
target_link_libraries(client PUBLIC ${ZeroMQ_LIBRARY} Threads::Threads)