#ifndef MY_OBJ_PLAYER_BULLET_H
#define MY_OBJ_PLAYER_BULLET_H

struct my_player_bullet {
    float speed;
    float x;
    float y;
};

struct k_object *my_player_bullet_create(float x, float y, int face);

#endif
