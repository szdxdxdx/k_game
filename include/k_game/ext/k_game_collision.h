#ifndef K_GAME_COLLISION_H
#define K_GAME_COLLISION_H

struct k_float_vec2;
struct k_room;
struct k_object;

/**
 * TODO Docs
 */
struct k_collision_box;

/* region [room_add_collision_box_manager] */

int k_room_add_collision_manager(void);

/* endregion */

/* region [object_add_collision_box] */

struct k_collision_rectangle_config {

    int group_id;

    struct k_float_vec2 *position;

    float offset_x1;
    float offset_y1;
    float offset_x2;
    float offset_y2;
};

struct k_collision_box *k_object_add_collision_rectangle(struct k_object *object, struct k_collision_rectangle_config *config);

struct k_collision_circle_config {

    int group_id;

    struct k_float_vec2 *position;

    float offset_cx;
    float offset_cy;
    float r;
};

struct k_collision_box *k_object_add_collision_circle(struct k_object *object, struct k_collision_circle_config *config);

void k_object_del_collision_box(struct k_collision_box *box);

/* endregion */

/* region [collision_box_get] */

struct k_object *k_collision_box_get_object(struct k_collision_box *box);

/* endregion */

/* region [collision_check] */

struct k_collision_box *k_collision_check_rectangle(int group_id, float x1, float y1, float x2, float y2);

struct k_collision_box *k_collision_check_circle(int group_id, float x, float y, float r);

/* endregion */

#endif
