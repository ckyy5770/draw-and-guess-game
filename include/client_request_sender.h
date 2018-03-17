//
// Created by chuilian on 3/17/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_REQUEST_SENDER_H
#define DRAW_AND_GUESS_GAME_CLIENT_REQUEST_SENDER_H

#include <string>
#include "zhelper.hpp"
#include "util.h"

/**
 * counterpart for server side request responder
 */
class client_request_sender{
public:
    client_request_sender(const std::string & ip, const std::string & port):
            port{port},
            ip{ip},
            zmq_context{0},
            zmq_socket_req{zmq_context, ZMQ_REQ}
    {};

    /**
     * connect the client to server request responder port
     * start() should be called prior to any send() call
     */
    void start(){
        try{
            zmq_socket_req.connect("tcp://" + ip + ":" + port);
        }catch(std::exception& ex){
            logger("zmq request socket bind to port " + port + " fail.");
            exit(1);
        }
    }

    /**
     * send a message to server, return the reply from server. The server is guaranteed to give a reply
     * @param msg
     * @return
     */
    std::string send(const std::string & msg){
        s_send(zmq_socket_req, msg);
        logger("message sent from client: " + msg);
        return s_recv(zmq_socket_req);
    }

private:
    // ZMQ socket context
    zmq::context_t zmq_context;

    // ZMQ reply socket
    zmq::socket_t zmq_socket_req;

    // server address
    std::string ip;

    // server request responder port
    std::string port;
};

#endif //DRAW_AND_GUESS_GAME_CLIENT_REQUEST_SENDER_H
