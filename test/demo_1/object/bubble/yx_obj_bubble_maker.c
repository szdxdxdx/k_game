#include <stdlib.h>

#include "../_internal.h"

static int my_rand(int min, int max) {
    return min + rand() % (max - min + 1);
}

static void yx_bubble_maker_create_bubble(struct k_object *object, int timeout_diff) {

    int padding = 32;
    float rand_x = (float)my_rand(padding, k_room_get_width() - padding);
    float rand_y = (float)my_rand(padding, k_room_get_height() - padding);

    yx_bubble_create(rand_x, rand_y);

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 200);
}

struct k_object *yx_obj_bubble_maker_create(const struct yx_obj_bubble_maker_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble_maker));
    struct yx_obj_bubble_maker *bubble_maker = k_object_get_data(object);

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 200);

    return object;
}
