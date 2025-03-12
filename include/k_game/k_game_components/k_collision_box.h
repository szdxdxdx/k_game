#ifndef K_GAME_COMPONENT_COLLISION_BOX_H
#define K_GAME_COMPONENT_COLLISION_BOX_H

enum k_collision_box_type {
    K_COLLISION_BOX_RECTANGLE,
    K_COLLISION_BOX_CIRCLE,
};

struct k_collision_box_rectangle_config {
    enum k_collision_box_type box_type;

    float x, y;
    float offset_x, offset_y;
};

struct k_collision_box_config {
    enum k_collision_box_type box_type;

    float *x, *y;
    float offset_x, offset_y;
};



#endif
