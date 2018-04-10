//
// Created by chuilian on 4/8/18.
//

#ifndef DRAW_AND_GUESS_GAME_SERVER_PLAYER_LIST_H
#define DRAW_AND_GUESS_GAME_SERVER_PLAYER_LIST_H

#include "./game_player.h"
#include <unordered_map>
#include <unordered_set>
#include <string>

class server_player_list{
public:
    server_player_list()
    {};

    int register_new_player(std::string name){
        if(player_names.find(name) != player_names.end()){
            // name already exists
            return -1;
        }else{
            // construct a new player object
            game_player new_player(name);

            // assign a id
            int new_id = next_id++;

            new_player.palyer_id = new_id;

            // insert this player to map
            player_map[new_id] = new_player;

            // claim this name is used
            player_names.insert(name);

            // return the id
            return new_id;
        }
    }

    game_player* get_player_by_id(int id){
        auto search = player_map.find(id);
        if(search != player_map.end()){
            // found
            return &(search->second);
        }else{
            return nullptr;
        }
    }


private:
    // id, player stats
    std::unordered_map<int, game_player> player_map;

    // player name set
    std::unordered_set<std::string> player_names;

    // next pre-allocated id
    int next_id = 1;

};

#endif //DRAW_AND_GUESS_GAME_SERVER_PLAYER_LIST_H
