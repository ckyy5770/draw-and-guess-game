//
// Created by Chuilian Kong on 3/3/2018.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H
#define DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H

#include <string>
#include <exception>
#include "zhelper.hpp"
#include "util.h"


/**
 * Publish messages to a specific group of clients, the topic should be group ID.
 */
class server_publisher{
public:
    /**
     * ctor
     * @param port
     */
    server_publisher(const std::string & port) :
            zmq_context{0},
            zmq_socket_pub{zmq_context, ZMQ_PUB},
            port{port}
    {};


    /**
     * Start to listen to the port
     * You should always start the publisher before send() any messages
     */
    void start(){
        try{
            zmq_socket_pub.bind("tcp://*:" + port);
        }catch(std::exception& ex){
            logger("zmq publisher socket bind to port " + port + " fail.");
            exit(1);
        }

        logger("zmq publisher socket successfully bind to port " + port + ".");
    }


    /**
     * Send one message of a specific topic to the subscribers
     * @param topic
     * @param msg
     */
    void send(std::string topic, std::string msg){
        s_sendmore(zmq_socket_pub, topic);
        s_send(zmq_socket_pub, msg);

        logger("message sent through publisher- topic: " + topic + " message: " + msg);
    }
private:

    // ZMQ socket context
    zmq::context_t zmq_context;

    // ZMQ publisher socket
    zmq::socket_t zmq_socket_pub;

    // PUB socket binding port
    std::string port;

};

#endif //DRAW_AND_GUESS_GAME_SERVER_PUBLISHER_H
