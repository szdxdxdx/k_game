
#include "object/bubble/yx_obj_bubble_maker.h"
#include "object/bubble/yx_obj_bubble.h"

#include "utils/yx_math.h"

static void yx_bubble_maker_create_bubble(struct k_object *object, int timeout_diff) {

    float padding = 32;
    float rand_x = yx_rand(padding, k_room_get_w() - padding);
    float rand_y = yx_rand(padding, k_room_get_h() - padding);

    yx_obj_bubble_create(rand_x, rand_y);

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 100);
}

struct k_object *yx_obj_bubble_maker_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_bubble_maker));

    k_object_add_alarm_callback(object, yx_bubble_maker_create_bubble, 100);
    return object;
}
