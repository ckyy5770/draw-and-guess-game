//
// Created by Chuilian Kong on 3/3/2018.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H
#define DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H

#include <string>
#include <exception>
#include "zhelper.hpp"
#include "util.h"

class server_publisher{
public:
    server_publisher(std::string port)
            : zmq_context{0},
              zmq_socket_pub{zmq_context, ZMQ_PUB},
              port{port}
    {};

    void start(){
        try{
            zmq_socket_pub.bind("tcp://*:" + port);
        }catch(std::exception& ex){
            logger("zmq publisher socket bind to port " + port + " fail.");
            exit(1);
        }

        logger("zmq publisher socket successfully bind to port " + port + ".");
    }

    void send(std::string topic, std::string msg){
        s_sendmore(zmq_socket_pub, topic);
        s_send(zmq_socket_pub, msg);

        logger("message sent through publisher- topic: " + topic + " message: " + msg);
    }
private:
    zmq::context_t zmq_context;
    zmq::socket_t zmq_socket_pub;

    std::string port;

};

#endif //DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H
