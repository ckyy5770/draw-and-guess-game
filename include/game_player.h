//
// Created by chuilian on 4/8/18.
//

#ifndef DRAW_AND_GUESS_GAME_PLAYER_T_H
#define DRAW_AND_GUESS_GAME_PLAYER_T_H

#include <string>
#include <cstring>
/**
     * this struct contains information of one player
     */
struct game_player{
    // 0 - invalid, waiting for server to assign a valid id
    // other - valid player_id
    int palyer_id = 0;

    // name str length cannot exceed 256
    // and this must be provided when the struct initialized
    char player_name[256];

    // the group id it belongs to
    // 0 - lobby
    // other - game room number
    int group_id = 0;

    // the game score of this player
    int score = 0;

    // the ready state of this player
    bool is_ready = false;
    /**
     * ctor
     * @param name
     */
    explicit game_player(const std::string & name){
        memcpy(player_name, name.c_str(), name.size());
    }

    game_player(){};

    bool operator==(const game_player& other){
        return palyer_id == other.palyer_id;
    }
};
#endif //DRAW_AND_GUESS_GAME_PLAYER_T_H
