#ifndef K_GAME_COMPONENT_COLLISION_BOX_H
#define K_GAME_COMPONENT_COLLISION_BOX_H

struct k_collision_box;

/* region [room_add_collision_box_manager] */

int k_room_add_collision_box_manager(struct k_room *room);

/* endregion */

/* region [object_add_collision_box] */

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

struct k_collision_box_config {

    enum k_collision_box_type box_type;

    union {
        struct k_collision_box_rectangle_config rectangle;

        struct k_collision_box_circle_config circle;
    };
};

struct k_collision_box *k_object_add_collision_box(struct k_object *object, struct k_collision_box_config *config);

void k_object_del_collision_box(struct k_collision_box *box);

/* endregion */

/* region [collision_check] */

struct k_collision_box *k_collision_check_rectangle(int layer, float x1, float y1, float x2, float y2);

struct k_collision_box *k_collision_check_circle(int layer, float x, float y, float r);

/* endregion */

#endif
