
#define K_LOG_TAG "yx:object:rival"
#include "k_log.h"

#include "k_game.h"

#include "config/yx_config_collision_group.h"

#include "object/weapon/yx_obj_weapon_i.h"
#include "object/rival/yx_obj_rival.h"
#include "object/text_particle/yx_obj_text_particle.h"
#include "utils/yx_math.h"

static void yx__obj_rival_on_step_hit_bullet_collision(struct k_object *object) {
    struct yx_obj_rival *rival = k_object_get_data(object);

    struct k_collision_box *bullet_box = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_BULLET, rival->hp_collision_box);
    if (NULL == bullet_box)
        return;

    struct yx_obj_bullet_i *bullet = k_object_get_data(k_collision_box_get_object(bullet_box));
    struct yx_obj_bullet_on_hit_result hit_result;
    yx_obj_bullet_on_hit(bullet, &hit_result);

    {
        struct yx_obj_text_particle_config config;
        config.x = rival->x;
        config.y = rival->y;
        config.vx = yx_rand(10.0f, 20.0f) * ((rand() % 2) ? 1.0f : -1.0f);
        config.vy = yx_rand(10.0f, 20.0f) * ((rand() % 2) ? 1.0f : -1.0f);
        config.color = 0xee3333ff;
        yx_obj_text_particle_create(&config, "-%d", (int)hit_result.damage);
    }

    rival->vx_knockback += hit_result.vx_knockback;
    rival->vy_knockback += hit_result.vy_knockback;
}

int yx__obj_rival_on_create_add_collision(struct yx_obj_rival *rival) {

    struct k_collision_rect_config config;
    config.x = &rival->x;
    config.y = &rival->y;
    config.group_id = YX_CONFIG_COLLISION_GROUP_FIGHTER;
    config.offset_x1 = -16.0f;
    config.offset_y1 = -24.0f;
    config.offset_x2 =  16.0f;
    config.offset_y2 =  12.0f;
    rival->hp_collision_box = k_object_add_collision_rect(rival->object, &config);
    if (NULL == rival->hp_collision_box)
        return -1;

    if (NULL == k_object_add_step_callback(rival->object, yx__obj_rival_on_step_hit_bullet_collision))
        return -1;

    return 0;
}
