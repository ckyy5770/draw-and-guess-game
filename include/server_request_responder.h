//
// Created by chuilian on 3/14/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_REQUEST_RESPONDER_H
#define DRAW_AND_GUESS_GAME_SERVER_REQUEST_RESPONDER_H

#include <string>
#include <boost/algorithm/string.hpp>
#include <exception>
#include "zhelper.hpp"
#include "util.h"
#include <unordered_map>

class server_request_responder{
public:
    server_request_responder(std::string port) :
            zmq_context{0},
            zmq_socket_rep{zmq_context, ZMQ_REP},
            port(port)
    {};

    /**
     * Start to listen to the port, and receiving messages
     * Note this function should run in an independent thread
     * since it contains a infinite loop to keep receiving messages
     */
    void start(){
        init_action_map();
        try{
            zmq_socket_rep.bind("tcp://*:" + port);
        }catch(std::exception& ex){
            logger("zmq reply socket bind to port " + port + " fail.");
            exit(1);
        }

        logger("zmq reply socket successfully bind to port " + port + ".");

        while(true){
            // receive one request from the client
            std::string request = s_recv(zmq_socket_rep);
            logger("received request: " + request);
            // process the request
            std::string reply = process_request(request);
            // send back the result
            s_send(zmq_socket_rep, reply);
            logger("sent reply: " + reply);
        }
    }
private:

    // ZMQ socket context
    zmq::context_t zmq_context;

    // ZMQ reply socket
    zmq::socket_t zmq_socket_rep;

    // REP socket binding port
    std::string port;

    // a map that contains all the respond logic for different requests
    std::unordered_map<std::string, std::string (*)(std::string, std::string, std::string)> action_map;


    /**
     * process one request
     * @param request
     * @return the result string that should be sent back to client
     */
    std::string process_request(std::string request){
        std::vector<std::string> parts;
        boost::split(parts, request, [](char c){return c=='|';});
        // retrieve request type, message body, related client id and group id
        std::string request_type = parts[1];
        std::string body = parts[2];
        std::string client_id = parts[3];
        std::string group_id = parts[4];
        // get action function according to request type
        auto action = action_map[request_type];
        return action(body, client_id, group_id);
    }


    /**
     * bind action functions to request type strings.
     * This function should run when starting the responder
     */
    void init_action_map(){
        action_map["NEW_GAME_ROOM"] = &action_new_game_room;
    }


    /********************action functions*****************/


    /**
     * create a new game room
     * @param body
     * @param client_id
     * @param group_id
     * @return
     */
    std::string action_new_game_room(std::string body, std::string client_id, std::string group_id){

    }

};


#endif //DRAW_AND_GUESS_GAME_SERVER_REQUEST_RESPONDER_H
