#include <limits.h>
#include "./demo_1.h"

/* region [player] */

struct my_player {
    float x, y;
    struct k_component *WASD;
    struct k_component *spr_rdr;
};

static void my_create_player(void) {

    struct k_object *obj_player = k_object_create(sizeof(struct my_player));

    struct my_player *player = k_object_get_data(obj_player);
    player->x = 300.0f;
    player->y = 300.0f;

    struct k_component_type *WASD = k_component_type_find("k/WASD");
    struct k_WASD_config WASD_config;
    WASD_config.key_up    = 'W';
    WASD_config.key_left  = 'A';
    WASD_config.key_down  = 'S';
    WASD_config.key_right = 'D';
    WASD_config.speed     = 150.0f;
    WASD_config.x         = &player->x;
    WASD_config.y         = &player->y;
    player->WASD = k_object_add_component(obj_player, WASD, &WASD_config);

    struct k_component_type *renderer = k_component_type_find("k/spr-rdr");
    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = k_sprite_find("player/run");
    renderer_config.z_index = 0;
    renderer_config.x       = &player->x;
    renderer_config.y       = &player->y;
    player->spr_rdr = k_object_add_component(obj_player, renderer, &renderer_config);
}

/* endregion */

static int fn_room_init(void *params) {
    struct k_room *room = k_get_current_room();

    k_room_add_draw_callback(room, NULL, k_room_clean_canvas, INT_MIN);

    my_create_player();
    return 0;
}

struct k_room *my_create_room_1(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_init = fn_room_init;

    return k_room_create(&config, NULL);
}
