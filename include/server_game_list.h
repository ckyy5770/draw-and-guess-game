//
// Created by chuilian on 4/8/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_GAME_LIST_H
#define DRAW_AND_GUESS_GAME_SERVER_GAME_LIST_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "./server_game_room.h"

class server_game_list{
public:
    server_game_list()
    {};

    int create_new_game(std::string name){

        // allocate id
        int id = next_id++;

        // create the instance in map
        game_list[id] = server_game_room(name);

        // reset id
        game_list[id].group_id = id;

        return id;
    }
private:
    // id, game state
    std::unordered_map<int, server_game_room> game_list;

    // next available id
    int next_id = 1;
};
#endif //DRAW_AND_GUESS_GAME_SERVER_GAME_LIST_H
