//
// Created by chuilian on 4/10/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_GAME_ROOM_H
#define DRAW_AND_GUESS_GAME_CLIENT_GAME_ROOM_H
#include <string>
#include <cstring>
#include <vector>
#include "./game_player.h"

/**
 * the game state from a player's perspective,
 */
struct client_game_room{

    // the group id of this game room
    // 0 - lobby
    // other - game room number
    int group_id = 0;

    // game room name
    std::string group_name;

    // a list of players in this room
    std::vector<game_player> player_list;

    // is the game on going?
    bool is_game_end = true;

    int drawer_id = -1;


    client_game_room(std::string game_room_name):
            group_name(game_room_name)
    {};

    client_game_room(){};
};
#endif //DRAW_AND_GUESS_GAME_CLIENT_GAME_ROOM_H
