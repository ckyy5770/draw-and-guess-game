//
// Created by chuilian on 4/10/18.
//

#ifndef DRAW_AND_GUESS_GAME_CLIENT_GAME_LIST_H
#define DRAW_AND_GUESS_GAME_CLIENT_GAME_LIST_H

#include <vector>
#include "client_game_room.h"

struct client_game_list{
    std::vector<client_game_room> room_list;
};
#endif //DRAW_AND_GUESS_GAME_CLIENT_GAME_LIST_H
