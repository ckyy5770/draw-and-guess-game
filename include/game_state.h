//
// Created by chuilian on 3/17/18.
//

#ifndef DRAW_AND_GUESS_GAME_GAME_STATE_H
#define DRAW_AND_GUESS_GAME_GAME_STATE_H

#include <string>
#include <cstring>
#include <list>

/**
 * the game state from a player's perspective,
 * server maintains a game_state for each game room,
 * server game state may use a subset of states that this class provides
 */
class game_state{
public:

    game_state(std::string game_room_name):
            group_name(game_room_name)
    {};
private:
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
    };

    // the group id of this game room
    // 0 - lobby
    // other - game room number
    int group_id = 0;

    // game room name
    std::string group_name;

    // a list of players in this room
    std::list<game_player> player_list;

    /** Game lobby does not need states below **/

    // is the game on going?
    bool is_game_end = true;

    int drawer_id = -1;

    /** Other Game rooms does not need states below **/

    // game room list
    std::list<game_state> room_list;


};
#endif //DRAW_AND_GUESS_GAME_GAME_STATE_H
