//
// Created by chuilian on 3/14/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H
#define DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H

#include <sys/socket.h>
#include <string>
#include <exception>
#include "util.h"


/**
 * drawing listener only listen to drawing information, i.e. the position of a new drawn pixel.
 */
class server_drawing_listener{
public:
    /**
     * ctor
     * @param port
     */
    server_drawing_listener(const std::string & port){
        if ((socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            logger("cannot create udp socket");
            throw std::runtime_error("cannot create udp socket");
        }

    }

private:
    // UDP socket file descriptor
    int socket_udp;

    // drawing_listener socket binding port
    std::string port;

};

#endif //DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H
