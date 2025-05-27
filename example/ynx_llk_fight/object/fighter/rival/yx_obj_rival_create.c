
#include <stdlib.h>

#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "sprite/yx_spr.h"

#include "object/fighter/rival/yx_obj_rival.h"
#include "config/yx_config_arena_blackboard.h"
#include "object/particle/yx_obj_shadow.h"
#include "object/particle/yx_fx_fighter_appear.h"
#include "object/weapon/yx_obj_weapon_i.h"
#include "utils/yx_math.h"

struct yx_obj_rival *yx_obj_rival_create(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival));

    struct yx_obj_rival *rival = k_object_get_data(object);
    rival->object = object;

    rival->hp = 3.0f;
    rival->x = x;
    rival->y = y;
    rival->vx_movement  = 0.0f;
    rival->vy_movement  = 0.0f;
    rival->vx_knockback = 0.0f;
    rival->vy_knockback = 0.0f;
    rival->spr_idle = yx_spr_llk_idle;
    rival->spr_run  = yx_spr_llk_run;
    rival->spr_dead = yx_spr_llk_dead;
    rival->face = (rand() % 2) ? -1 : 1;
    rival->attack_state = YX_OBJ_RIVAL_STATE_PATROL;

    if (0 != yx__obj_rival_on_create_init_movement(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_init_attack(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_init_collision(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_init_draw(rival))
        goto err;
    if (0 != yx__obj_rival_on_create_init_debug(rival))
        goto err;

    {
        struct k_position_config position_config;
        position_config.parent  = NULL;
        position_config.world_x = &rival->x;
        position_config.world_y = &rival->y;
        position_config.local_x = rival->x;
        position_config.local_y = rival->y;
        rival->position = k_object_add_position(object, &position_config);
        if (NULL == rival->position)
            goto err;
    }

    {
        rival->object_shadow = yx_obj_shadow_create(yx_spr_shadow_1, rival->position, 0, -15);
        if (NULL == rival->object_shadow)
            goto err;
    }

    rival->blackboard = k_room_blackboard_get(YX_ARENA_BLACKBOARD_KEY);
    if (NULL == rival->blackboard) {
        k_log_error("object yx_obj_rival requires room to provide blackboard '%s'", YX_ARENA_BLACKBOARD_KEY);
        goto err;
    }

    rival->blackboard->rival_wave_spawner->rivals_num += 1;

    return rival;

err:
    k_object_destroy(object);
    k_log_error("failed to create object yx_obj_rival");
    return NULL;
}

void yx_obj_rival_destroy(struct yx_obj_rival *rival) {

    if (NULL != rival->weapon) {
        yx_obj_rival_weapon_destroy(rival->weapon);
    }
    if (NULL != rival->object_shadow) {
        k_object_destroy(rival->object_shadow);
    }

    k_object_destroy(rival->object);
}

/* ------------------------------------------------------------------------ */

/* 创建敌人时的粒子效果 */
struct yx_obj_rival_appear_fx {
    float x;
    float y;
    struct yx_fx_fighter_appear *fx;
    struct k_callback *cb_step;
};

static void yx__obj_rival_spawner_fx_callback(struct k_object *object) {
    struct yx_obj_rival_appear_fx *fx = k_object_get_data(object);

    struct yx_obj_rival *rival = yx_obj_rival_create(fx->x, fx->y);
    if (NULL == rival) {
        k_log_warn("failed to create rival");
    }
}

void yx_obj_rival_spawn(float x, float y) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival_appear_fx));
    if (NULL == object)
        goto err;

    struct yx_obj_rival_appear_fx *fx = k_object_get_data(object);
    fx->x = x;
    fx->y = y;

    fx->fx = yx_fx_fighter_appear_create(x, y, yx__obj_rival_spawner_fx_callback, object);
    if (NULL == fx->fx)
        goto err;

    return;

err:
    k_object_destroy(object);
}

/* ------------------------------------------------------------------------ */

struct yx_obj_rival_wave_spawner *yx_obj_rival_wave_spawner_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival));
    if (NULL == object)
        goto err;

    struct yx_obj_rival_wave_spawner *spawner = k_object_get_data(object);

    spawner->blackboard = k_room_blackboard_get(YX_ARENA_BLACKBOARD_KEY);
    if (NULL == spawner->blackboard) {
        k_log_error("object yx_obj_rival_wave_spawner requires room to provide blackboard '%s'", YX_ARENA_BLACKBOARD_KEY);
        goto err;
    }

    spawner->blackboard->rival_wave_spawner = spawner;

    spawner->rivals_num = 0;

    yx_obj_rival_wave_spawner_new_wave(spawner);

    return spawner;

err:
    k_log_error("failed to create object yx_obj_rival_wave_spawner");
    return NULL;
}

void yx_obj_rival_wave_spawner_new_wave(struct yx_obj_rival_wave_spawner *spawner) {

    float x_min = 0.0f;
    float y_min = 0.0f;
    float x_max = k_room_get_w();
    float y_max = k_room_get_h();

    int i = 0;
    for (; i < 10; ++i) {
        float x = yx_rand(x_min, x_max);
        float y = yx_rand(y_min, y_max);
        yx_obj_rival_spawn(x, y);
    }
}
