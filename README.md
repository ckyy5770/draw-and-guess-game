# draw-and-guess-game

## implementation

### overview

#### server

The main task of server in this online game is to keep receiving messages of each client, and respond them accordingly. For example, before game start, a client may ask for joining a existing game. It will send a request to the server, and the server should decide whether or not this client should join the game. And then send back a response to it. If the server accepted the request, it may need to notify every other clients within that game. 

So the sever should have the capability of receiving messages from all clients and responding to any one of them and notifying specific group of clients. Based on those requirements, we design the server as follows:

* the server should have a request responder which listens to a specific port. Clients will send all their requests to that port, and the server is required to send a response back to the client after some decision is made. The response may or may not contains further information about the requested resources. **We use ZMQ REQ-REP sockets to implement this listener.** Example Usages: 

    * a client requests to join a existing game.
    
    * a client requests to leave a existing game.
    
    * a client requests to create a new game.
    
    * a client requests to set their ready state to "ready".
    
    * a client requests to set their ready state to "not ready".
    
    * a client requests to take a guess.

* the server should have a drawing information listener which listens to a specific port. Why would we need another listener if we already have a request responder? In this specific game, we have a very special message flow between client and server, that is, the drawing information. Every single pixel that a drawer draws on its white board should be synchronized to all the players in that game. The drawing information should be sent to the sever by the drawer, and the server will publish this information to all players in that game. This drawing information messages are in huge amount, a drawer may draw thousands of pixels within one second, and the sever do not need to acknowledge its receiving of every single messages. A lost of a few amount of drawn pixels won't affect the overall figure that the drawer is drawing. Therefore, this is a perfect place where we use UDP as the underlying transport protocol due to its fast and simple delivery with minimum amount of overhead. **Since ZMQ REQ-REP sockets use TCP as their underlying transport protocol, we won't use them to implement this listener. Instead, we use raw UDP socket to implement this listener.** Example Usages:

    * a client (drawer) reports a drawn pixel to the server.
    
* the server should have a publisher which publishes messages to a specific group of clients. **We use ZMQ PUB-SUB sockets to implement our server publisher.** We use topic to specify the game ID that a client is in, so that the client is only able to listen to the messages that is belong to its game. And thus the server is able to host multiple games at the same time. Since ZMQ PUB-SUB socket do a filtering on the publisher side, so we don't need to worry about a waste of bandwidth on the server side. Example usages:

    * the server publishes a reported drawn pixel to all players in a game.
    
    * the server publishes a new guess issued by some player to all players in a game
    
    * the server publishes a new player to all players in a game.
    
    * the server publishes a left player to all players in a game.
    
    * the server publishes a new ready state change to all players in a game.
    
    * the server publishes a new game state change to all players in a game. (game start, game end - new winner)


#### client

the client can be divided into two parts in general. The first part is the graphic interface, and the second part is network communication. The network communication part should be the counterpart of the server side code. And the graphic interface should handle all user input and display game states.

##### networking

This part should be counterpart of the server side networking code, I will just briefly introduce the sockets we used in clients.

* the client should have a request sender which binds to the server request responder port. Clients will send all their requests to that port and receive the response from server. The server is required to send back a response for each client request. **We use ZMQ REQ-REP sockets to implement this sender.**

* the client should have a subscriber which subscribe to specific game, and keep receiving all updated information related to that game. **We use ZMQ PUB-SUB sockets to implement our client subscriber.**
