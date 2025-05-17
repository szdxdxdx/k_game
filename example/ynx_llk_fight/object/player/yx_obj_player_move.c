
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "object/player/yx_obj_player.h"

#if 0

/* region [state] */

static void yx_obj_player_on_idle_state_enter(struct k_object *object);
static void yx_obj_player_on_idle_state_step(struct k_object *object);
static struct k_state_machine_state STATE_IDLE = {
    yx_obj_player_on_idle_state_enter,
    yx_obj_player_on_idle_state_step,
    NULL
};

static void yx_obj_player_on_running_state_enter(struct k_object *object);
static void yx_obj_player_on_running_state_step(struct k_object *object);
static struct k_state_machine_state STATE_RUNNING = {
    yx_obj_player_on_running_state_enter,
    yx_obj_player_on_running_state_step,
    NULL
};

/* region [idle] */

static void yx_obj_player_on_idle_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_idle);
}

static void yx_obj_player_on_idle_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    if (player->next_x != player->x || player->next_y != player->y) {
        k_state_machine_change_state(player->state_machine, STATE_RUNNING);
    }
}

/* endregion */

/* region [running] */

static void yx_obj_player_on_running_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);
}

static void yx_obj_player_on_running_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    if (player->next_x == player->x) {
        if (player->next_y == player->y) {
            k_state_machine_change_state(player->state_machine, STATE_IDLE);
            return;
        }
    }
    else {
        if (player->next_x < player->x) {
            k_sprite_renderer_flip_x(player->spr_rdr, 0);
        } else {
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        }
    }

    if (player->next_y != player->y) {
        k_sprite_renderer_set_z_layer(player->spr_rdr, (int)player->next_y);
    }

    k_position_set_local_position(player->position, player->next_x, player->next_y);
}

/* endregion */

/* endregion */

#endif



int yx__obj_player_on_create_add_movement(struct yx_obj_player *player) {


    return 0;
}
