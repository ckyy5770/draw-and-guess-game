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
        // create room
        server_game_room new_game(name);

        // allocate id
        int id = next_id++;

        // move the instance to map
        game_list[id] = new_game;

        return id;
    }
private:
    // id, game state
    std::unordered_map<int, server_game_room> game_list;

    // next available id
    int next_id = 1;
};
#endif //DRAW_AND_GUESS_GAME_SERVER_GAME_LIST_H
