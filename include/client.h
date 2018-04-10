//
// Created by chuilian on 4/10/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_H
#define DRAW_AND_GUESS_GAME_CLIENT_H

#include <thread>
#include "client_subscriber.h"
#include "client_request_sender.h"
#include "client_drawing_sender.h"
#include "client_game_room.h"
#include "client_game_list.h"

class client{
public:
    client(const std::string & ip, const std::string & server_responder_port, const std::string & server_drawing_linstener_port, const std::string & server_pub_port):
            request_sender{ip, server_responder_port},
            drawing_sender{ip, server_drawing_linstener_port},
            subscriber{ip, server_pub_port}
    {}

    void start(){
        // start senders -> binding ports
        request_sender.start();
        drawing_sender.start();
        // set subscribe group to 0 -> lobby
        subscriber.reset_group("0");
        // start subscriber in a thread
        t_subscriber = std::thread(&client_subscriber::start, &subscriber);
    }


private:
    // request sender
    client_request_sender request_sender;

    // drawing sender
    client_drawing_sender drawing_sender;

    // subscriber
    client_subscriber subscriber;

    // subscriber thread
    std::thread t_subscriber;

    // current game room info
    client_game_room game_room;

    // game room list
    client_game_list game_list;

};
#endif //DRAW_AND_GUESS_GAME_CLIENT_H
