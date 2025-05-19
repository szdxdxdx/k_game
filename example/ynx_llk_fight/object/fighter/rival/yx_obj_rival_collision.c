
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"

#include "object/weapon/yx_obj_weapon_i.h"
#include "object/fighter/rival/yx_obj_rival.h"
#include "object/text_particle/yx_obj_text_particle.h"
#include "utils/yx_math.h"

/* 被子弹击中时，创建一个受伤的文字粒子效果 */
static void yx__obj_rival_create_text_particle_on_hit(struct yx_obj_rival *rival, struct yx_obj_bullet_on_hit_result *hit_result) {
    struct yx_float_vec2 v_knockback = {
            .x = hit_result->vx_knockback,
            .y = hit_result->vy_knockback
        };
        struct yx_float_vec2 v_dir_knockback = yx_float_vec2_normalize(v_knockback);
        struct yx_float_vec2 v_text = yx_float_vec2_perp_right(v_dir_knockback);
        if (rand() % 2)
            v_text = yx_float_vec2_neg(v_text);

        v_text = yx_float_vec2_scale(v_text, yx_rand(40.0f, 60.0f));

        struct yx_obj_text_particle_config config;
        config.x = rival->x;
        config.y = rival->y;
        config.vx = v_text.x;
        config.vy = v_text.y;
        config.color = 0xff1111ff;
        yx_obj_text_particle_create(&config, "-%d", (int)hit_result->damage);
}

/* 判断自身有没有被子弹击中 */
void yx__obj_rival_on_step_hit_bullet_collision(struct yx_obj_rival *rival) {

    struct k_collision_box *bullet_box = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_BULLET, rival->hp_collision_box);
    if (NULL == bullet_box)
        return;

    struct yx_obj_bullet_i *bullet = k_object_get_data(k_collision_box_get_object(bullet_box));
    struct yx_obj_bullet_on_hit_result hit_result;
    yx_obj_bullet_on_hit(bullet, &hit_result);

    yx__obj_rival_create_text_particle_on_hit(rival, &hit_result);

    rival->vx_knockback += hit_result.vx_knockback;
    rival->vy_knockback += hit_result.vy_knockback;
}

int yx__obj_rival_on_create_add_collision(struct yx_obj_rival *rival) {

    {
        struct k_collision_rect_config config;
        config.x = &rival->x;
        config.y = &rival->y;
        config.group_id = YX_CONFIG_COLLISION_GROUP_RIVAL_HP;
        config.offset_x1 = -16.0f;
        config.offset_y1 = -24.0f;
        config.offset_x2 = 16.0f;
        config.offset_y2 = 12.0f;
        rival->hp_collision_box = k_object_add_collision_rect(rival->object, &config);
        if (NULL == rival->hp_collision_box)
            return -1;
    }

    return 0;
}
