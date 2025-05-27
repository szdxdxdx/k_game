#include <assert.h>

#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_arena_blackboard.h"
#include "config/yx_config_collision_group.h"
#include "config/yx_config_z_index.h"

#include "sprite/yx_spr.h"

#include "object/fighter/player/yx_obj_player.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/particle/yx_obj_alert_marker.h"
#include "object/weapon/apple/yx_obj_weapon_apple.h"

#include "utils/yx_math.h"

/* region [attack_sm] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object);
static void yx__obj_rival_on_state_patrol_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_PATROL = {
    .on_enter  = yx__obj_rival_on_state_patrol_enter,
    .on_update = yx__obj_rival_on_state_patrol_update,
    .on_leave  = NULL,
};

static void yx__obj_rival_on_state_attack_enter(struct k_object *object);
static void yx__obj_rival_on_state_attack_update(struct k_object *object);
static struct yx_state_machine_state YX_STATE_ATTACK = {
    .on_enter  = yx__obj_rival_on_state_attack_enter,
    .on_update = yx__obj_rival_on_state_attack_update,
    .on_leave  = NULL,
};

/* region [patrol] */

static void yx__obj_rival_on_state_patrol_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->attack_state = YX_OBJ_RIVAL_STATE_PATROL;
}

static void yx__obj_rival_on_state_patrol_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    struct yx_obj_player *player = rival->blackboard->player;

    struct yx_float_vec2 dir = yx_float_vec2_new(player->x - rival->x, player->y - rival->y);

    if (yx_float_vec2_length(dir) <= YX__OBJ_RIVAL_AGGRO_RADIUS) { /* 与玩家距离太近，切换成攻击状态 */
        yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_ATTACK);
        return;
    }

    yx_obj_rival_weapon_aim_at(rival->weapon, rival->target_position_x, rival->target_position_y);
}

/* endregion */

/* region [attack] */

static void yx__obj_rival_on_state_attack_enter(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->attack_state = YX_OBJ_RIVAL_STATE_ATTACK;

    yx_obj_alert_marker_create(rival->position, -30, -16); /* 感叹号气泡 */

    rival->attack_timer = yx_rand(1.0f, 3.5f);
}

static void yx__obj_rival_on_state_attack_update(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    struct yx_obj_player *player = rival->blackboard->player;

    struct yx_float_vec2 dir = yx_float_vec2_new(player->x - rival->x, player->y - rival->y);

    if (yx_float_vec2_length(dir) >= YX__OBJ_RIVAL_LOSE_TARGET_RADIUS) { /* 与玩家距离太远，切换成巡逻状态 */
        yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_PATROL);
        return;
    }

    yx_obj_rival_weapon_aim_at(rival->weapon, player->x, player->y);

    float dt = k_time_get_step_delta();
    rival->attack_timer -= dt;
    if (rival->attack_timer <= 0.0f) {
        rival->attack_timer = yx_rand(1.5f, 5.5f);

        yx_obj_rival_weapon_attack(rival->weapon);
    }
}

/* endregion */

/* endregion */

static void yx__obj_rival_on_step_attack(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);
    yx_state_machine_tick(&rival->attack_sm);
}

int yx__obj_rival_on_create_init_attack(struct yx_obj_rival *rival) {

    rival->cb_on_step_attack = k_object_add_step_callback(rival->object, yx__obj_rival_on_step_attack);
    if (NULL == rival->cb_on_step_attack)
        return -1;

    rival->weapon = yx_obj_rival_weapon_apple_create();
    if (NULL == rival->weapon)
        return -1;

    yx_state_machine_init(rival->object, &rival->attack_sm);
    yx_state_machine_change_state(&rival->attack_sm, &YX_STATE_PATROL);

    return 0;
}
