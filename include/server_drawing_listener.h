//
// Created by chuilian on 3/14/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H
#define DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <exception>
#include "util.h"
#include "server_publisher.h"
#include <boost/algorithm/string.hpp>


/**
 * drawing listener only listen to drawing information, i.e. the position of a new drawn pixel.
 */
class server_drawing_listener{
public:
    /**
     * ctor
     * @param port
     */
    server_drawing_listener(const std::string & port, const server_publisher & publisher) :
            port{port},
            publisher{publisher}
    {}

    /**
     * create socket, start to listen to the drawing information port
     *
     */
    void start(){
        struct sockaddr_in myaddr;      /* our address */
        struct sockaddr_in remaddr;     /* remote address */
        socklen_t addrlen = sizeof(remaddr);            /* length of addresses */
        int recvlen;                    /* # bytes received */
        char buf[BUFSIZE];     /* receive buffer */

        /* create a UDP socket */
        if ((socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                logger("cannot create udp socket");
                return;
        }

        /* bind the socket to any valid IP address and a specific port */
        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        myaddr.sin_port = htons(std::stoi(port));

        if (bind(socket_udp, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
                logger("udp socket bind failed");
                return;
        }

        /* now loop, receiving data and printing what we received */
        while (true) {
            std::string msg;
            logger("waiting on port " + port);
            recvlen = recvfrom(socket_udp, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
            logger("received " + std::to_string(recvlen) + " bytes.");
            if (recvlen > 0) {
                    buf[recvlen] = 0;
                    msg = std::string(buf);
                    logger("received message: " + msg);
            }
            // publish this message to all subscribers
            publish_new_drawing(msg);
        }
    }

private:
    // UDP socket file descriptor
    int socket_udp;

    // drawing_listener socket binding port
    std::string port;

    // UDP socket buffer size 
    size_t BUFSIZE = 2048;

    // ref to a server publisher, being used to publish new drawing to all players in a game
    const server_publisher & publisher;

    /**
     * publish the new drawing pixel via server publisher
     *
     */
    void publish_new_drawing(const std::string & msg){
        // extract client group id - parts[4]
        std::vector<std::string> parts;
        boost::split(parts, msg, [](char c){return c=='|';});
        // publish the new drawing
        publisher.publish(parts[4], msg);
    }

};

#endif //DRAW_AND_GUESS_GAME_SERVER_DRAWING_LISTENER_H
