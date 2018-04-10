//
// Created by chuilian on 4/10/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_GAME_STATE_H
#define DRAW_AND_GUESS_GAME_SERVER_GAME_STATE_H
#include <string>
#include <cstring>
#include <vector>
#include "./game_player.h"

/**
 * the game state from a server's perspective,
 * server maintains a game_state for each game room
 */
struct server_game_room{

    // the group id of this game room
    // 0 - lobby
    // other - game room number
    int group_id = 0;

    // game room name
    std::string group_name;

    // a list of players (specified by id) in this room
    std::vector<int> player_list;

    // is the game on going?
    bool is_game_end = true;

    int drawer_id = -1;


    server_game_room(std::string game_room_name):
            group_name(game_room_name)
    {};

    server_game_room(){}

};
#endif //DRAW_AND_GUESS_GAME_SERVER_GAME_STATE_H
