
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "sprite/yx_spr.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/weapon/apple/yx_obj_weapon_apple.h"

struct yx_obj_player *yx_obj_player_create(const struct yx_obj_player_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == object)
        goto err;

    struct yx_obj_player *player = k_object_get_data(object);

    player->object = object;
    player->x = config->x;
    player->y = config->y;

    player->key_up    = 'W';
    player->key_left  = 'A';
    player->key_down  = 'S';
    player->key_right = 'D';
    player->speed     = 200.0f;

    player->spr_idle = yx_spr_ynx_idle;
    player->spr_run  = yx_spr_ynx_run;

    if (0 != yx__obj_player_on_create_add_draw(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_movement(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_collision(player))
        goto err;
    if (0 != yx__obj_player_on_create_add_debug(player))
        goto err;

    player->weapon = yx_obj_weapon_apple_create();

    return player;

err:
    k_object_destroy(object);
    k_log_error("failed to create yx_obj_player");
    return NULL;
}
