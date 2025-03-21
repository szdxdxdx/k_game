#ifndef MY_GAME__OBJ_PLAYER_BULLET_H
#define MY_GAME__OBJ_PLAYER_BULLET_H

#include "k_game.h"

struct my_player_bullet {
    float speed;
    float x;
    float y;
};

struct k_object *my_player_bullet_create(float x, float y, int face);

#endif
