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

### message format

#### overview

* any messages in this application should follow the same message format when communication with each other

* in general, a message should has following information:

    * message origin: CLI or SRV

    * request or response type: e.g. NEW_GAME_ROOM

    * message body that is specific for its type.

    * related client id.

    * related client group id.

* the message should be partitioned by '|', and provided information should strict follows the order listed above

#### detailed message list

##### NEW_DRAWN_PIXEL

* CLI to SRV : `CLI|UDP_NEW_DRAWN_PIXEL|[x-axis]&[y-axis]|[CLIENT_ID]|[CLIENT_GROUP_ID]`

* SRV to CLI : topic - `[CLIENT_GROUP_ID]` content - `SRV|PUB_NEW_DRAWN_PIXEL|[x-axis]&[y-axis]|[CLIENT_ID]|[CLIENT_GROUP_ID]`

##### NEW_PLAYER

* CLI to SRV: `CLI|REQUEST_NEW_PLAYER|[CLIENT_IP]&[PLAYER_NAME]|NULL|NULL`

* SRV to CLI: `SRV|REPLY_NEW_PLAYER|CLIENT_IP]&[PLAYER_NAME]|[ASSIGNED_ID]|NULL`

##### JOIN_GAME

* CLI to SRV: `CLI|REQUEST_JOIN_GAME|[GAME_ROOM_ID]|[CLIENT_ID]|[CURRENT_GROUP_ID]]`

* SRV to CLI: `SRV|REPLY_JOIN_GAME|[ACCEPTED]&[GAME_STATE]|[CLIENT_ID]|[UPDATED_GROUP_ID]`

* SRV to CLI: `SRV|REPLY_JOIN_GAME|[DECLINED]&[REASON]|[CLIENT_ID]|[CURRENT_GROUP_ID]`

##### GAME_STATE_UPDATE

* SRV to CLI: topic - `[CORRESPONDING_GROUP_ID]` content - `SRV|PUB_GAME_STATE_UPDATE|[GAME_STATE]|NULL|[CORRESPONDING_GROUP_ID]`

##### CHANGE_READY_STATE

* CLI to SRV: `CLI|REQUEST_CHANGE_READY_STATE|[UPDATED_READY_STATE]|[CLIENT_ID]|[GROUP_ID]`

* SRV to CLI: `SRV|REPLY_CHANGE_READY_STATE|[[ACCEPTED]&[UPDATED_READY_STATE]|[CLIENT_ID]|[GROUP_ID]`

* SRV to CLI: `SRV|REPLY_CHANGE_READY_STATE|[[DECLINED]&[CURRENT_READY_STATE]|[CLIENT_ID]|[GROUP_ID]`

##### CREATE_GAME

* CLI to SRV: `CLI|REQUEST_CREATE_GAME|[NEW_GAME_NAME]]|[CLIENT_ID]|0`

* SRV to CLI: `SRV|REPLY_CREATE_GAME|[ACCEPTED]&[NEW_GAME_ID]|[CLIENT_ID]|0`

* SRV to CLI: `SRV|REPLY_CREATE_GAME|[DECLINED]&[REASON]|[CLIENT_ID]|0`

#####

### major procedures

#### initialize server

* start a server publisher, pass the reference to drawing listener and request responder so that they can use it.

* create a server_state instance which contains a player list and room list (game state list)

* create the game lobby and add it into the room list

* start a request responder in an independent thread.

* start a drawing listener in an independent thread.

#### initialize client

* start a client request sender, drawing sender

* start a subscriber, add default channel (game lobby) to subscription list

* initialize game state for this client

* start UI

* ask the player name in the UI

* send a REQUEST_NEW_PLAYER to the server, waiting server to assign a ID

* send a REQUEST_JOIN_GAME to the server, to join the game lobby, waiting server to send game lobby state to it

#### join a game

* client send a REQUEST_JOIN_GAME to the server, with the room id

* server received the request and decide whether this client should join the game, if the request is accepted, the server should update the game state of new room and old room and send back the new room's game state, if the request is declined, the server should send a message telling the client the reason.

#### game state update

* if there is any game state changes in server side, the server should publish this information to the corresponding group

#### ready state change

* client send a REQUEST_CHANGE_READY_STATE to the server, with the current state

* server received the request and decide whether the changing should be performed, if the request is accepted, the server should update the ready state of that player, then the game state of that room, and publish this new game state change to everyone in the room, if the request is declined, the server should send a message contains the reason.

#### report a new drawn pixel

* client send NEW_DRAWN_PIXEL to the server, and the server will publish it to all players in the room. The client don't expect a reply from server.

#### leave a game room

* client will send a REQUEST_JOIN_GAME with room id 0 (game lobby)

#### create a game

* client send a REQUEST_CREATE_GAME with game name to the server

* server received the request and decide whether the game should be created, if the request is accepted, the server should create the game and return the new game info to the client

### data schema

we use [flatbuffers](https://github.com/google/flatbuffers/tree/master/include/flatbuffers) to do serialization and deserialization of our major game data, here are data schemas we use: