
#include <stdlib.h>

#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"
#include "object/fighter/player/yx_obj_player.h"
#include "object/particle/yx_obj_text_particle.h"
#include "object/particle/yx_obj_particle_on_hit.h"
#include "utils/yx_math.h"

/* region [bubble] */

static void yx__obj_player_touch_bubble(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

#if 0
    float x1 = player->x;
    float y1 = player->y;
    float x2 = player->weapon->x;
    float y2 = player->weapon->y;
    struct k_collision_box *box = k_collision_check_rectangle(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    if (NULL != box)
        yx_bubble_pop(k_collision_box_get_object(box));
#endif
}

/* endregion */

static void yx__obj_rival_create_text_particle_on_hit(struct yx_obj_player *player, struct yx_bullet_on_hit_result *hit_result) {
    struct yx_float_vec2 v_knockback = {
        .x = hit_result->vx_knockback,
        .y = hit_result->vy_knockback
    };
    struct yx_float_vec2 v_dir_knockback = yx_float_vec2_normalize(v_knockback);
    struct yx_float_vec2 v_text = yx_float_vec2_perp_right(v_dir_knockback);
    if (rand() % 2)
        v_text = yx_float_vec2_neg(v_text);

    v_text = yx_float_vec2_scale(v_text, yx_rand(40.0f, 60.0f));

    struct yx_obj_particle_text_on_hit_config config;
    config.x = player->x;
    config.y = player->y;
    config.vx = v_text.x;
    config.vy = v_text.y;
    yx_obj_particle_text_on_hit_create(&config, "-%d", (int)hit_result->damage);
}

/* 判断自身有没有被子弹击中 */
void yx__obj_player_on_step_check_hit_bullet(struct yx_obj_player *player) {

    struct k_collision_box *bullet_box = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET, player->hp_collision_box);
    if (NULL == bullet_box)
        return;

    struct yx_obj_player_bullet *bullet = k_object_get_data(k_collision_box_get_object(bullet_box));
    struct yx_bullet_on_hit_result hit_result;
    yx_obj_player_bullet_on_hit(bullet, &hit_result);

    yx__obj_rival_create_text_particle_on_hit(player, &hit_result);
    yx_obj_particle_on_hit_create(player->x, player->y);

    player->vx_knockback += hit_result.vx_knockback;
    player->vy_knockback += hit_result.vy_knockback;
}

int yx__obj_player_on_create_init_collision(struct yx_obj_player *player) {

    struct k_collision_rect_config config;
    config.x = &player->x;
    config.y = &player->y;
    config.group_id = YX_CONFIG_COLLISION_GROUP_PLAYER_HP;
    config.offset_x1 = -14.0f;
    config.offset_y1 = -20.0f;
    config.offset_x2 =  14.0f;
    config.offset_y2 =  10.0f;
    player->hp_collision_box = k_object_add_collision_rect(player->object, &config);
    if (NULL == player->hp_collision_box)
        return -1;

    return 0;
}
