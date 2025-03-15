#ifndef K_GAME_COMPONENT_COLLISION_BOX_H
#define K_GAME_COMPONENT_COLLISION_BOX_H

enum k_collision_box_type {
    K_COLLISION_BOX_RECTANGLE,
    K_COLLISION_BOX_CIRCLE,
};

struct k_collision_box_rectangle_config {

    enum k_collision_box_type box_type;

    float *x;
    float *y;

    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_box_circle_config {

    enum k_collision_box_type box_type;

    float *x;
    float *y;

    float offset_x;
    float offset_y;
    float r;
};

void k_collision_check_rectangle(float x1, float y1, float x2, float y2);

#endif
