#include "k_game_keyboard.h"
#include "k_game_time.h"
#include "k_game_component.h"

#include "k_game_components/k_WASD.h"

#include "./k_components_def.h"

struct k_WASD {

    float *x;
    float *y;

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_right;

    float speed;
};

static void WASD_step(struct k_component *component) {
    struct k_WASD *WASD = k_component_get_data(component);

    float delta_time = k_get_step_delta();

    if (k_key_down(WASD->key_up))
        *(WASD->y) -= WASD->speed * delta_time;
    if (k_key_down(WASD->key_down))
        *(WASD->y) += WASD->speed * delta_time;
    if (k_key_down(WASD->key_left))
        *(WASD->x) -= WASD->speed * delta_time;
    if (k_key_down(WASD->key_right))
        *(WASD->x) += WASD->speed * delta_time;
}

static int WASD_init(struct k_component *component, void *params) {
    struct k_WASD *WASD = k_component_get_data(component);
    struct k_WASD_config *config = params;

    WASD->key_up    = config->key_up;
    WASD->key_down  = config->key_down;
    WASD->key_left  = config->key_left;
    WASD->key_right = config->key_right;
    WASD->speed     = config->speed;
    WASD->x         = config->x;
    WASD->y         = config->y;

    if (NULL == k_component_add_step_callback(component, WASD_step))
        return -1;

    return 0;
}

int k__component_def_WASD(void) {

    struct k_component_type_config config = K_COMPONENT_TYPE_CONFIG_INIT;
    config.type_name = "k/WASD";
    config.data_size = sizeof(struct k_WASD);
    config.fn_init = WASD_init;

    return NULL != k_component_define(&config) ? 0 : -1;
}
