//
// Created by chuilian on 3/17/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_DRAWING_SENDER_H
#define DRAW_AND_GUESS_GAME_CLIENT_DRAWING_SENDER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include "util.h"


/**
 * counter part for server side drawing listener
 * send new drawn pixel over UDP
 */
class client_drawing_sender{
public:
    /**
     * ctor
     */
    client_drawing_sender(const std::string & ip, const std::string & port):
            port{port},
            ip{ip}
    {};


    /**
     * create UDP socket and fill in server address info
     * start() should be call prior to any send() calls
     */
    void start(){
        // create a UDP socket
        if ((socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            logger("cannot create socket");
            return;
        }

        /* bind to an arbitrary return address */
        /* because this is the client side, we don't care about the address */
        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(0);

        if (bind(socket_udp, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
            logger("socket bind failed");
            return;
        }


        /* fill in the server's address and data */
        memset((char*)&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(std::stoi(port));
        inet_pton(AF_INET, ip.c_str(), &(servaddr.sin_addr));

    }

    /**
     * send new drawn pixel message to server
     * @param msg
     */
    void send(std::string msg){
        if (sendto(socket_udp, msg.c_str(), msg.size(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            logger("send new drawn pixel message failed");
            return;
        }
    }
private:
    // UDP socket
    int socket_udp;

    // server ip address
    std::string ip;

    // server receiving port for drawing information
    std::string port;

    // server address struct
    struct sockaddr_in servaddr;

    // client address struct
    struct sockaddr_in myaddr;

};

#endif //DRAW_AND_GUESS_GAME_CLIENT_DRAWING_SENDER_H
