#ifndef K_GAME_COMPONENT_COLLISION_BOX_H
#define K_GAME_COMPONENT_COLLISION_BOX_H

struct k_collision_box;

/* region [room_add_collision_box_manager] */

int k_room_add_collision_manager(struct k_room *room);

/* endregion */

/* region [object_add_collision_box] */

struct k_collision_rectangle_config {

    uint8_t box_type;

    float *x;
    float *y;

    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, struct k_collision_rectangle_config *config);

struct k_collision_circle_config {

    uint8_t box_type;

    float *x;
    float *y;

    float offset_x;
    float offset_y;
    float r;
};

struct k_collision_box *k_object_add_collision_circle(struct k_object *object, struct k_collision_circle_config *config);

void k_object_del_collision_box(struct k_collision_box *box);

/* endregion */

/* region [collision_check] */

struct k_collision_box *k_collision_check_rectangle(int layer, float x1, float y1, float x2, float y2);

struct k_collision_box *k_collision_check_circle(int layer, float x, float y, float r);

/* endregion */

#endif
