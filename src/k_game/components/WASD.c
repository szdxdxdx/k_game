#include "k_game_keyboard.h"
#include "k_game_time.h"
#include "k_game_component.h"
#include "k_game_components/WASD.h"

struct WASD_data {

    /* 确保 `position` 是结构体的第一个成员，
     * 使得 `k_component_get_data()` 的结果能
     * 类型转化为 `struct k_component_WASD`。
     */
    struct k_component_WASD position;

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_right;

    float speed;
};

static void WASD_step(struct k_component *component) {
    struct WASD_data *data = k_component_get_data(component);

    float delta_time = k_get_step_delta();

    if (k_is_key_down(data->key_up))
        data->position.y -= data->speed * delta_time;
    if (k_is_key_down(data->key_down))
        data->position.y += data->speed * delta_time;
    if (k_is_key_down(data->key_left))
        data->position.x -= data->speed * delta_time;
    if (k_is_key_down(data->key_right))
        data->position.x += data->speed * delta_time;
}

static int WASD_init(struct k_component *component, void *params) {
    struct k_component_WASD_config *config = params;
    struct WASD_data *data = k_component_get_data(component);

    data->key_up     = config->key_up;
    data->key_down   = config->key_down;
    data->key_left   = config->key_left;
    data->key_right  = config->key_right;
    data->speed      = config->speed;
    data->position.x = config->x;
    data->position.y = config->y;

    if (NULL == k_component_add_step_callback(component, WASD_step))
        return -1;

    return 0;
}

int k__define_component_type_WASD(void) {

    struct k_component_type_config config;
    config.type_name = "WASD";
    config.data_size = sizeof(struct WASD_data);
    config.fn_init = WASD_init;
    config.fn_fini = NULL;

    if (NULL == k_define_component_type(&config))
        return -1;
    else
        return 0;
}
