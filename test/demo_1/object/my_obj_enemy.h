#ifndef MY_OBJ_ENEMY_H
#define MY_OBJ_ENEMY_H

struct my_enemy {
    float x;
    float y;

    float next_x;
    float next_y;

    struct k_component *WASD;
    struct k_sprite_renderer *spr_rdr;
};

struct k_object *my_enemy_create(float x, float y);

#endif
