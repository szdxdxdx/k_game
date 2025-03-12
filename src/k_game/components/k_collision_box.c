#include <stdint.h>

#include "k_game_component.h"

#include "k_game_components/k_collision_box.h"
#include "./k_components_def.h"

/* region [box_type_def] */

struct k_collision_box_rectangle {
    uint8_t box_type;

    float *x, *y;
    float offset_x1, offset_y1;
    float offset_x2, offset_y2;
};

struct k_collision_box_circle {
    uint8_t box_type;

    float *x, *y;
    float offset_x, offset_y;
    float r;
};

union k_collision_box {
    uint8_t box_type;
    struct k_collision_box_rectangle rectangle;
    struct k_collision_box_circle    circle;
};

/* endregion */

int box_init(struct k_component *component, void *params) {
    struct k_collision_box_config *config = params;


    return 0;
}

int k__component_def_collision_box(void) {

    struct k_component_type_config config = K_COMPONENT_TYPE_CONFIG_INIT;
    config.type_name = "k/C-box";
    config.data_size = sizeof(union k_collision_box);
    config.fn_init   = box_init;

    return NULL != k_component_define(&config) ? 0 : -1;
}
