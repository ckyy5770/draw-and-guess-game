cmake_minimum_required(VERSION 3.9)
project(draw_and_guess_game)

set(CMAKE_CXX_STANDARD 11)

include_directories(./lib/ZMQ)

add_executable(server main/server_main.cpp src/server/server_publisher.cpp include/util.h include/server_publisher.h include/server_request_responder.h include/server_drawing_listener.h include/server_player_list.h include/game_player.h include/server_game_list.h include/server_game_room.h include/server.h)

add_executable(client main/client_main.cpp include/util.h include/client_drawing_sender.h include/client_request_sender.h include/client_subscriber.h include/game_player.h include/client_game_room.h include/client.h include/client_game_list.h)
