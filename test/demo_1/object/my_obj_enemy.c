#include "./my_object.h"

#include "../sprite/my_sprite.h"

struct k_object *my_enemy_create(float x, float y) {
    struct k_room *room = k_get_current_room();

    struct k_object *object = k_object_create(sizeof(struct my_enemy), room);

    struct my_enemy *enemy = k_object_get_data(object);
    enemy->x = x;
    enemy->y = y;

    struct k_component_type *WASD = k_component_type_find("k/WASD");
    struct k_WASD_config WASD_config;
    WASD_config.key_up    = 'I';
    WASD_config.key_left  = 'J';
    WASD_config.key_down  = 'K';
    WASD_config.key_right = 'L';
    WASD_config.speed     = 150.0f;
    WASD_config.x         = &enemy->x;
    WASD_config.y         = &enemy->y;
    enemy->WASD = k_object_add_component(object, WASD, &WASD_config);

    struct k_sprite_renderer_config renderer_config;
    renderer_config.sprite  = my_spr_enemy_attack;
    renderer_config.z_index = -1;
    renderer_config.x       = &enemy->x;
    renderer_config.y       = &enemy->y;
    enemy->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);

    struct k_collision_rectangle_config collision_rect;
    collision_rect.x = &enemy->x;
    collision_rect.y = &enemy->y;
    collision_rect.offset_x1 = -35.0f;
    collision_rect.offset_y1 = -50.0f;
    collision_rect.offset_x2 =  35.0f;
    collision_rect.offset_y2 =   5.0f;
    k_object_add_collision_rectangle(object, &collision_rect);

    return object;
}
