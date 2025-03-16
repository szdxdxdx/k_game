#include <limits.h>
#include "./demo_1.h"

/* region [player] */

struct my_enemy {
    float x, y;
    struct k_component *WASD;
};

static void my_create_enemy(void) {

    struct k_object *obj_enemy = k_object_create(sizeof(struct my_enemy));

    struct my_enemy *enemy = k_object_get_data(obj_enemy);
    enemy->x = 500.0f;
    enemy->y = 300.0f;

    struct k_component_type *WASD = k_component_type_find("k/WASD");
    struct k_WASD_config WASD_config;
    WASD_config.key_up    = 'I';
    WASD_config.key_left  = 'J';
    WASD_config.key_down  = 'K';
    WASD_config.key_right = 'L';
    WASD_config.speed     = 150.0f;
    WASD_config.x         = &enemy->x;
    WASD_config.y         = &enemy->y;
    enemy->WASD = k_object_add_component(obj_enemy, WASD, &WASD_config);

    struct k_component_type *renderer = k_component_type_find("k/spr-rdr");
    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = k_sprite_find("enemy/attack");
    renderer_config.z_index = 0;
    renderer_config.x       = &enemy->x;
    renderer_config.y       = &enemy->y;
    k_object_add_component(obj_enemy, renderer, &renderer_config);
}

/* endregion */

static int fn_room_init(void *params) {
    struct k_room *room = k_get_current_room();

    k_room_add_draw_callback(room, NULL, k_room_clean_canvas, INT_MIN);
    k_room_add_draw_callback(room, NULL, k_room_draw_grid, INT_MIN + 1);

    my_create_player(300.0f, 300.0f);
   // my_create_enemy();
    return 0;
}

struct k_room *my_create_room_1(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = fn_room_init;

    return k_room_create(&config, NULL);
}
