//
// Created by chuilian on 4/10/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_H
#define DRAW_AND_GUESS_GAME_SERVER_H

#include <thread>
#include "server_request_responder.h"
#include "server_drawing_listener.h"
#include "server_publisher.h"
#include "server_game_list.h"
#include "server_game_room.h"
#include "server_player_list.h"

class server{
public:
    server(const std::string & ip, const std::string & request_responder_port, const std::string & drawing_listener_port, const std::string & publisher_port):
            publisher{publisher_port},
            request_responder{request_responder_port},
            drawing_listener{drawing_listener_port, publisher}

    {}

    void start(){
        // start publisher -> binding ports
        publisher.start();

        // start request responder in a thread
        t_responder = std::thread(request_responder.start());

        // start drawing listener in a thread
        t_drawing_listener = std::thread(drawing_listener.start());
    }


private:
    // publisher
    server_publisher publisher;

    // request responder
    server_request_responder request_responder;

    // drawing listening
    server_drawing_listener drawing_listener;

    // request_responder thread
    std::thread t_responder;

    // drawing listener thread
    std::thread t_drawing_listener;

    // game room info
    server_game_list game_list;

    // player info
    server_player_list player_list;


};

#endif //DRAW_AND_GUESS_GAME_SERVER_H
