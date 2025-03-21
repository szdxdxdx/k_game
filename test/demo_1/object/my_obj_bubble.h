#ifndef MY_OBJ_BUBBLE_H
#define MY_OBJ_BUBBLE_H

struct my_bubble {
    struct k_sprite_renderer *spr_rdr;

    float x, y;
};

struct k_object *my_bubble_create(float x, float y);

#endif
