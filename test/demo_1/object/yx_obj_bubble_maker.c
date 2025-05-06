#include <stdlib.h>

#include "./yx_obj.h"

static float my_rand(float min, float max) {
    int min_ = (int)min;
    int max_ = (int)max;
    int r = min_ + rand() % (max_ - min_ + 1);
    return (float)r;
}

static void yx_bubble_maker_create_bubble(struct k_object *object, int timeout_diff) {

    float padding = 32;
    float rand_x = my_rand(padding, k_room_get_w() - padding);
    float rand_y = my_rand(padding, k_room_get_h() - padding);

    yx_obj_bubble_create(rand_x, rand_y);

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 100);
}

struct k_object *yx_obj_bubble_maker_create(const struct yx_obj_bubble_maker_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble_maker));
    struct yx_obj_bubble_maker *bubble_maker = k_object_get_data(object);

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 100);
    return object;
}
