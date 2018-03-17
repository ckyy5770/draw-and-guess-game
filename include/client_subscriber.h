//
// Created by chuilian on 3/17/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_SUBSCRIBER_H
#define DRAW_AND_GUESS_GAME_CLIENT_SUBSCRIBER_H

#include "zhelper.hpp"
#include <boost/algorithm/string.hpp>
#include <unordered_map>

class client_subscriber{
public:
    /**
     * ctor
     * @param ip server ip
     * @param port server publisher port
     */
    client_subscriber(std::string ip, std::string port):
            ip{ip},
            port{port},
            zmq_context{0},
            zmq_socket_sub{zmq_context, ZMQ_SUB}
    {};

    /**
     * connect to server publisher socket
     */
    void start(){
        zmq_socket_sub.connect("tcp://" + ip + ":" + port);
        // all subscriber must subscribe topic "SYSTEM"
        // and they can also subscribe topic [THEIR_GROUP_ID]
        zmq_socket_sub.setsockopt(ZMQ_SUBSCRIBE, "SYSTEM");
        zmq_socket_sub.setsockopt(ZMQ_SUBSCRIBE, group_id);
    }

    /**
     * reset the channel this subcriber is listening to
     * @param new_group_id
     */
    void reset_group(const std::string & new_group_id){
        zmq_socket_sub.setsockopt(ZMQ_UNSUBSCRIBE, group_id);
        group_id = new_group_id;
        zmq_socket_sub.setsockopt(ZMQ_SUBSCRIBE, group_id);
    }

    /**
     * receive one message and take an action for it
     */
    void receive(){
        // source address, we don't need it so just discard it
        std::string address = s_recv(zmq_socket_sub);
        // message content
        std::string contents = s_recv(zmq_socket_sub);
        // process this server request
        process_request(contents);
    }



private:
    // server ip
    std::string ip;

    // server publisher port
    std::string port;

    // ZMQ socket context
    zmq::context_t zmq_context;

    // ZMQ subscriber socket
    zmq::socket_t zmq_socket_sub;

    // GROUP ID of this client, initialized with "DEFAULT"
    // note "default group" doesn't exist actually,
    // the subscriber won't receive any message under DEFAULT group
    // to receive message of a specified group, use reset_group()
    std::string group_id = "DEFAULT";

    // a map that contains all the respond logic for different requests
    std::unordered_map<std::string, void (*)(const std::string &, const std::string &, const std::string &)> action_map;

    /**
     * process one server request
     * @param request
     */
    void process_request(const std::string & request){
        std::vector<std::string> parts;
        boost::split(parts, request, [](char c){return c=='|';});
        // retrieve request type, message body, related client id and group id
        std::string request_type = parts[1];
        std::string body = parts[2];
        std::string client_id = parts[3];
        std::string group_id = parts[4];
        // get action function according to request type
        auto action = action_map[request_type];
        action(body, client_id, group_id);
    }

    /**
     * bind action functions to request type strings.
     * This function should run when starting the responder
     */
    void init_action_map(){
        action_map["NEW_PLAYER"] = &action_new_player;
    }


    /********************action functions*****************/


    /**
     * add a new player to the game
     * @param body
     * @param client_id
     * @param group_id
     * @return
     */
    void action_new_player(const std::string & body, const std::string & client_id, const std::string & group_id){

    }
};
#endif //DRAW_AND_GUESS_GAME_CLIENT_SUBSCRIBER_H
