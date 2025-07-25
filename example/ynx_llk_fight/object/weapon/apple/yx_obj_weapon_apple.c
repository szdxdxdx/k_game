
#include <math.h>

#define K_LOG_TAG "yx:object:weapon_apple"
#include "k_log.h"

#include "k_game.h"

#include "object/weapon/apple/yx_obj_weapon_apple.h"
#include "sprite/yx_spr.h"
#include "config/yx_config_z_index.h"
#include "utils/yx_math.h"
#include "config/yx_config_collision_group.h"
#include "sound/yx_sound.h"
#include "object/particle/yx_obj_shadow.h"

/* region [player_weapon] */

/* region [bullet_apple] */

static void yx__obj_player_bullet_apple_on_hit(struct yx_obj_player_bullet *bullet, struct yx_bullet_on_hit_result *get_result) {
    struct yx_obj_player_bullet_apple *bullet_apple = (struct yx_obj_player_bullet_apple *)bullet;

    if(NULL != get_result) {
        get_result->vx_knockback = bullet_apple->vx * 0.75f;
        get_result->vy_knockback = bullet_apple->vy * 0.75f;
        get_result->damage = rand() % 2 ? 2.0f : 3.0f;
    }

    k_object_del_collision_box(bullet_apple->hit_box);
    bullet_apple->hit_box = NULL;

    bullet_apple->vx *= 0.3f;
    bullet_apple->vy *= 0.3f;
    bullet_apple->rotation_speed *= 0.12f;
    k_sprite_renderer_set_sprite(bullet_apple->spr_rdr, yx_spr_bullet_apple_crack);
    k_sprite_renderer_set_loop_count(bullet_apple->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bullet_apple->spr_rdr, k_object_destroy);

    k_object_destroy(bullet_apple->object_shadow);
    bullet_apple->object_shadow = NULL;

    k_sound_sfx_play(yx_sfx_apple_break[rand() % 3]);
}

static struct yx_obj_player_bullet_v_tbl yx__obj_player_bullet_apple_v_tbl = {
    .on_hit = yx__obj_player_bullet_apple_on_hit
};

static void yx__obj_player_bullet_apple_on_step(struct k_object *object) {
    struct yx_obj_player_bullet_apple *bullet_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    bullet_apple->x += bullet_apple->vx * dt;
    bullet_apple->y += bullet_apple->vy * dt;

    float padding = -30.0f;
    struct k_float_rect room_rect = {
        .x = padding,
        .y = padding,
        .w = k_room_get_w() - padding,
        .h = k_room_get_h() - padding
    };
    if ( ! yx_point_in_rect(bullet_apple->x, bullet_apple->y, &room_rect)) {
        k_object_destroy(bullet_apple->object_shadow);
        k_object_destroy(object);
        return;
    }

    float angle = k_sprite_renderer_get_rotation(bullet_apple->spr_rdr);
    angle += dt * bullet_apple->rotation_speed;
    k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);

    k_position_set_local_position(bullet_apple->position, bullet_apple->x, bullet_apple->y);

    {
        /* 玩家的苹果可以击碎敌人的苹果，此处是临时的实现方案 */

        if (NULL != bullet_apple->hit_box) {
            struct k_collision_box *hit = k_collision_check_box(YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET, bullet_apple->hit_box);
            if (NULL != hit) {
                struct yx_obj_rival_bullet *bullet = k_object_get_data(k_collision_box_get_object(hit));
                yx_obj_rival_bullet_on_hit(bullet, NULL);
                yx__obj_player_bullet_apple_on_hit(&bullet_apple->super, NULL);
                return;
            }
        }
    }
}

static struct yx_obj_player_bullet_apple *yx__obj_player_bullet_apple_create(struct yx_obj_player_weapon_apple *weapon_apple) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player_bullet_apple));
    if (NULL == object)
        goto err;

    struct yx_obj_player_bullet_apple *bullet_apple = k_object_get_data(object);
    bullet_apple->super.object = object;
    bullet_apple->super.v_tbl = &yx__obj_player_bullet_apple_v_tbl;

    {
        struct k_collision_rect_config config;
        config.x = &bullet_apple->x;
        config.y = &bullet_apple->y;
        config.group_id = YX_CONFIG_COLLISION_GROUP_PLAYER_BULLET;
        config.offset_x1 = -16.0f;
        config.offset_y1 = -16.0f;
        config.offset_x2 = 16.0f;
        config.offset_y2 = 16.0f;
        bullet_apple->hit_box = k_object_add_collision_rect(bullet_apple->super.object, &config);
        if (NULL == bullet_apple->hit_box)
            goto err;
    }

    bullet_apple->x = weapon_apple->x;
    bullet_apple->y = weapon_apple->y;

    bullet_apple->rotation_speed = yx_rand(450.0f, 700.0f);

    float sin_angle;
    float cos_angle;
    yx_calc_vector_direction(weapon_apple->x, weapon_apple->y, weapon_apple->aim_x, weapon_apple->aim_y, &cos_angle, &sin_angle);

    float speed = yx_rand(550.0f, 650.0f);
    bullet_apple->vx = cos_angle * speed;
    bullet_apple->vy = sin_angle * speed;

    {
        struct k_sprite_renderer_config config;
        config.x       = &bullet_apple->x;
        config.y       = &bullet_apple->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_BULLET;
        config.z_layer = 0;
        bullet_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == bullet_apple->spr_rdr)
            goto err;

        float angle = k_sprite_renderer_get_rotation(weapon_apple->spr_rdr);
        k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_player_bullet_apple_on_step))
        goto err;

    {
        struct k_position_config position_config;
        position_config.world_x = &bullet_apple->x;
        position_config.world_y = &bullet_apple->y;
        position_config.parent = NULL;
        position_config.local_x = bullet_apple->x;
        position_config.local_y = bullet_apple->y;
        bullet_apple->position = k_object_add_position(object, &position_config);
        if (NULL == bullet_apple->position)
            goto err;
    }

    {
        bullet_apple->object_shadow = yx_obj_shadow_create(yx_spr_shadow_2, bullet_apple->position, 0, -4);
        if (NULL == bullet_apple->object_shadow)
            goto err;
    }

    return bullet_apple;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: bullet apple");
    return NULL;
}

/* endregion */

/* region [weapon_apple] */

static void yx__obj_player_weapon_apple_fn_set_position(struct yx_obj_player_weapon *weapon, float x, float y, int z) {
    struct yx_obj_player_weapon_apple *weapon_apple = (struct yx_obj_player_weapon_apple *)weapon;
    weapon_apple->x = x;
    weapon_apple->y = y;
    k_sprite_renderer_set_z_layer(weapon_apple->spr_rdr, z);
}

static void yx__obj_player_weapon_apple_fn_aim_at(struct yx_obj_player_weapon *weapon, float x, float y) {
    struct yx_obj_player_weapon_apple *weapon_apple = (struct yx_obj_player_weapon_apple *)weapon;
    weapon_apple->aim_x = x;
    weapon_apple->aim_y = y;

    if (weapon_apple->aim_x != weapon_apple->x) {
        float angle = atanf((weapon_apple->aim_y - weapon_apple->y) / (weapon_apple->aim_x - weapon_apple->x));
        angle *= 180.0f / 3.1415926f;

        k_sprite_renderer_rotate(weapon_apple->spr_rdr, angle);

        if (weapon_apple->aim_x < weapon_apple->x) {
            k_sprite_renderer_flip_x(weapon_apple->spr_rdr, 1);
        }
        else {
            k_sprite_renderer_flip_x(weapon_apple->spr_rdr, 0);
        }
    }
}

static void yx__obj_player_weapon_apple_attack(struct yx_obj_player_weapon_apple *weapon_apple, int *in_out_ammo) {

    /*
    if (weapon_apple->ammo > 0) {
        yx__obj_player_bullet_apple_create(weapon_apple);
        weapon_apple->attack_cd_timer = weapon_apple->attack_cd_time;
        weapon_apple->ammo -= 1;

        k_sound_sfx_play(yx_sfx_fire);

        if (0 == weapon_apple->ammo) {
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 0.0f);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 0.0f);
        }
    }
    */

    if (*in_out_ammo <= 0)
        return;

    yx__obj_player_bullet_apple_create(weapon_apple);
    weapon_apple->attack_cd_timer = weapon_apple->attack_cd_time;

    *in_out_ammo -= 1;

    k_sound_sfx_play(yx_sfx_fire);
}

static void yx__obj_player_weapon_apple_on_key_down(struct yx_obj_player_weapon *weapon, int *in_out_ammo) {
    struct yx_obj_player_weapon_apple *weapon_apple = (struct yx_obj_player_weapon_apple *)weapon;

    yx__obj_player_weapon_apple_attack(weapon_apple, in_out_ammo);
}

static void yx__obj_player_weapon_apple_on_key_held(struct yx_obj_player_weapon *weapon, int *in_out_ammo) {
    struct yx_obj_player_weapon_apple *weapon_apple = (struct yx_obj_player_weapon_apple *)weapon;

   if (0.0f == weapon_apple->attack_cd_timer) {
        yx__obj_player_weapon_apple_attack(weapon_apple, in_out_ammo);
   }
}

static void yx__obj_player_weapon_apple_on_key_up(struct yx_obj_player_weapon *weapon, int *in_out_ammo) {
    (void)weapon;
    (void)in_out_ammo;
}

static void yx__obj_player_weapon_apple_destroy(struct yx_obj_player_weapon *weapon) {
    struct yx_obj_player_weapon_apple *weapon_apple = (struct yx_obj_player_weapon_apple *)weapon;
    k_object_destroy(weapon_apple->super.object);
}

static struct yx_obj_player_weapon_v_tbl yx__obj_player_weapon_apple_v_tbl = {
    .fn_destroy      = yx__obj_player_weapon_apple_destroy,
    .fn_set_position = yx__obj_player_weapon_apple_fn_set_position,
    .fn_aim_at       = yx__obj_player_weapon_apple_fn_aim_at,
    .on_key_down     = yx__obj_player_weapon_apple_on_key_down,
    .on_key_held     = yx__obj_player_weapon_apple_on_key_held,
    .on_key_up       = yx__obj_player_weapon_apple_on_key_up,
};

static void yx__obj_player_weapon_apple_on_step(struct k_object *object) {
    struct yx_obj_player_weapon_apple *weapon_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    if (/*weapon_apple->ammo > 0 && */weapon_apple->attack_cd_timer > 0.0f) {
        weapon_apple->attack_cd_timer -= dt;  /* CD 冷却 */

        if (weapon_apple->attack_cd_timer <= 0) {
            weapon_apple->attack_cd_timer = 0.0f; /* CD 冷却已结束 */
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 1.0f);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 1.0f);
        }
        else {
            /* CD 冷却中，苹果慢慢变大 */
            float scale = 1.0f - weapon_apple->attack_cd_timer / weapon_apple->attack_cd_time;
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, scale);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, scale);
        }
    }

    /*
    weapon_apple->ammo_timer += dt;
    if (weapon_apple->ammo_timer >= weapon_apple->ammo_cd_time) {
        weapon_apple->ammo += 1;
        weapon_apple->ammo_timer -= weapon_apple->ammo_cd_time;
    }
    */
}

struct yx_obj_player_weapon *yx_obj_player_weapon_apple_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player_weapon_apple));
    if (NULL == object)
        return NULL;

    struct yx_obj_player_weapon_apple *weapon_apple = k_object_get_data(object);

    weapon_apple->super.object = object;
    weapon_apple->super.v_tbl = &yx__obj_player_weapon_apple_v_tbl;

    weapon_apple->x = 0;
    weapon_apple->y = 0;
    weapon_apple->aim_x = 0;
    weapon_apple->aim_y = 0;

    weapon_apple->attack_cd_time = 0.35f;
    weapon_apple->attack_cd_timer = 0;

    /*
    weapon_apple->ammo = 1000;
    weapon_apple->ammo_timer = 0.0f;
    weapon_apple->ammo_cd_time = 2.0f;
    */

    {
        struct k_sprite_renderer_config config;
        config.x       = &weapon_apple->x;
        config.y       = &weapon_apple->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_WEAPON;
        config.z_layer = 0;
        weapon_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == weapon_apple->spr_rdr)
            goto err;
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_player_weapon_apple_on_step))
        goto err;

    return &weapon_apple->super;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: weapon apple");
    return NULL;
}

/* endregion */

/* endregion */

/* region [rival_weapon] */

/* region [bullet_apple] */

static void yx__obj_rival_bullet_apple_on_hit(struct yx_obj_rival_bullet *bullet, struct yx_bullet_on_hit_result *get_result) {
    struct yx_obj_rival_bullet_apple *bullet_apple = (struct yx_obj_rival_bullet_apple *)bullet;

    if(NULL != get_result) {
        get_result->vx_knockback = bullet_apple->vx * 0.75f;
        get_result->vy_knockback = bullet_apple->vy * 0.75f;
        get_result->damage = rand() % 2 ? 2.0f : 3.0f;
    }

    k_object_del_collision_box(bullet_apple->hit_box);
    bullet_apple->hit_box = NULL;

    bullet_apple->vx *= 0.3f;
    bullet_apple->vy *= 0.3f;
    bullet_apple->rotation_speed *= 0.12f;
    k_sprite_renderer_set_sprite(bullet_apple->spr_rdr, yx_spr_bullet_apple_crack);
    k_sprite_renderer_set_loop_count(bullet_apple->spr_rdr, 1);
    k_sprite_renderer_set_loop_callback(bullet_apple->spr_rdr, k_object_destroy);

    k_object_destroy(bullet_apple->object_shadow);
    bullet_apple->object_shadow = NULL;

    k_sound_sfx_play(yx_sfx_apple_break[rand() % 3]);
}

static struct yx_obj_rival_bullet_v_tbl yx__obj_rival_bullet_apple_v_tbl = {
    .on_hit = yx__obj_rival_bullet_apple_on_hit
};

static void yx__obj_rival_bullet_apple_on_step(struct k_object *object) {
    struct yx_obj_rival_bullet_apple *bullet_apple = k_object_get_data(object);

    float dt = k_time_get_step_delta();

    bullet_apple->x += bullet_apple->vx * dt;
    bullet_apple->y += bullet_apple->vy * dt;

    float padding = -30.0f;
    struct k_float_rect room_rect = {
        .x = padding,
        .y = padding,
        .w = k_room_get_w() - padding,
        .h = k_room_get_h() - padding
    };
    if ( ! yx_point_in_rect(bullet_apple->x, bullet_apple->y, &room_rect)) {
        k_object_destroy(bullet_apple->object_shadow);
        k_object_destroy(object);
        return;
    }

    float angle = k_sprite_renderer_get_rotation(bullet_apple->spr_rdr);
    angle += dt * bullet_apple->rotation_speed;
    k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);

    k_position_set_local_position(bullet_apple->position, bullet_apple->x, bullet_apple->y);
}

static struct yx_obj_rival_bullet_apple *yx__obj_rival_bullet_apple_create(struct yx_obj_rival_weapon_apple *weapon_apple) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival_bullet_apple));
    if (NULL == object)
        goto err;

    struct yx_obj_rival_bullet_apple *bullet_apple = k_object_get_data(object);
    bullet_apple->super.object = object;
    bullet_apple->super.v_tbl = &yx__obj_rival_bullet_apple_v_tbl;

    {
        struct k_collision_rect_config config;
        config.x = &bullet_apple->x;
        config.y = &bullet_apple->y;
        config.group_id = YX_CONFIG_COLLISION_GROUP_RIVAL_BULLET;
        config.offset_x1 = -16.0f;
        config.offset_y1 = -16.0f;
        config.offset_x2 = 16.0f;
        config.offset_y2 = 16.0f;
        bullet_apple->hit_box = k_object_add_collision_rect(object, &config);
        if (NULL == bullet_apple->hit_box)
            goto err;
    }

    bullet_apple->x = weapon_apple->x;
    bullet_apple->y = weapon_apple->y;

    bullet_apple->rotation_speed = yx_rand(100.0f, 600.0f);

    float sin_angle;
    float cos_angle;
    yx_calc_vector_direction(weapon_apple->x, weapon_apple->y, weapon_apple->aim_x, weapon_apple->aim_y, &cos_angle, &sin_angle);

    float speed = yx_rand(300.0f, 380.0f);
    bullet_apple->vx = cos_angle * speed;
    bullet_apple->vy = sin_angle * speed;

    {
        struct k_sprite_renderer_config config;
        config.x       = &bullet_apple->x;
        config.y       = &bullet_apple->y;
        config.sprite  = yx_spr_bullet_apple;
        config.z_group = YX_CONFIG_Z_GROUP_BULLET;
        config.z_layer = 0;
        bullet_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == bullet_apple->spr_rdr)
            goto err;

        float angle = k_sprite_renderer_get_rotation(weapon_apple->spr_rdr);
        k_sprite_renderer_rotate(bullet_apple->spr_rdr, angle);
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_rival_bullet_apple_on_step))
        goto err;

    {
        struct k_position_config position_config;
        position_config.world_x = &bullet_apple->x;
        position_config.world_y = &bullet_apple->y;
        position_config.parent = NULL;
        position_config.local_x = bullet_apple->x;
        position_config.local_y = bullet_apple->y;
        bullet_apple->position = k_object_add_position(object, &position_config);
        if (NULL == bullet_apple->position)
            goto err;
    }

    {
        bullet_apple->object_shadow = yx_obj_shadow_create(yx_spr_shadow_2, bullet_apple->position, 0, -4);
        if (NULL == bullet_apple->object_shadow)
            goto err;
    }

    return bullet_apple;

err:
    k_object_destroy(object);
    k_log_error("failed to create object: yx_obj_rival_bullet_apple");
    return NULL;
}

/* endregion */

/* region [weapon_apple] */

static void yx__obj_rival_weapon_apple_set_position(struct yx_obj_rival_weapon *weapon, float x, float y, int z) {
    struct yx_obj_rival_weapon_apple *weapon_apple = (struct yx_obj_rival_weapon_apple *)weapon;
    weapon_apple->x = x;
    weapon_apple->y = y;
    k_sprite_renderer_set_z_layer(weapon_apple->spr_rdr, z);
}

static void yx__obj_rival_weapon_apple_aim_at(struct yx_obj_rival_weapon *weapon, float x, float y) {
    struct yx_obj_rival_weapon_apple *weapon_apple = (struct yx_obj_rival_weapon_apple *)weapon;
    weapon_apple->aim_x = x;
    weapon_apple->aim_y = y;

    if (weapon_apple->aim_x != weapon_apple->x) {
        float angle = atanf((weapon_apple->aim_y - weapon_apple->y) / (weapon_apple->aim_x - weapon_apple->x));
        angle *= 180.0f / 3.1415926f;

        k_sprite_renderer_rotate(weapon_apple->spr_rdr, angle);

        if (weapon_apple->aim_x < weapon_apple->x) {
            k_sprite_renderer_flip_x(weapon_apple->spr_rdr, 1);
        }
        else {
            k_sprite_renderer_flip_x(weapon_apple->spr_rdr, 0);
        }
    }
}

static void yx__obj_rival_weapon_apple_attack(struct yx_obj_rival_weapon *weapon) {
    struct yx_obj_rival_weapon_apple *weapon_apple = (struct yx_obj_rival_weapon_apple *)weapon;

    {
        /* 函数在瞄准点周围（小于 r 的圆范围内）取一个随机点，让 ai 瞄准得并不是那么准 */

        float r = 10.0f;
        float theta = yx_rand(0.0f, 2.0f * 3.1415926f);
        float radius = sqrtf(yx_rand(0.0f, 1.0f) * (r * r));
        weapon_apple->aim_x += radius * cosf(theta);
        weapon_apple->aim_y += radius * sinf(theta);
    }

    yx__obj_rival_bullet_apple_create(weapon_apple);
    weapon_apple->attack_cd_timer = weapon_apple->attack_cd_time;

    k_sound_sfx_play(yx_sfx_fire);
}

static void yx__obj_rival_weapon_apple_destroy(struct yx_obj_rival_weapon *weapon) {
    struct yx_obj_rival_weapon_apple *weapon_apple = (struct yx_obj_rival_weapon_apple *)weapon;
    k_object_destroy(weapon_apple->super.object);
}

static struct yx_obj_rival_weapon_v_tbl yx__obj_rival_weapon_apple_v_tbl = {
    .fn_destroy = yx__obj_rival_weapon_apple_destroy,
    .fn_set_position = yx__obj_rival_weapon_apple_set_position,
    .fn_aim_at = yx__obj_rival_weapon_apple_aim_at,
    .fn_attack = yx__obj_rival_weapon_apple_attack,
};

static void yx__obj_rival_weapon_apple_on_step(struct k_object *object) {
    struct yx_obj_rival_weapon_apple *weapon_apple = k_object_get_data(object);

    if (weapon_apple->attack_cd_timer <= 0.0f)
        return;

    float dt = k_time_get_step_delta();
    weapon_apple->attack_cd_timer -= dt;

    if (weapon_apple->attack_cd_timer <= 0.0f) {
        weapon_apple->attack_cd_timer = 0.0f;
        k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 1.0f);
        k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 1.0f);
    } else {
        float t = 0.2f;
        if (weapon_apple->attack_cd_timer > t) {
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, 0.0f);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, 0.0f);
        }
        else {
            float scale = 1.0f - weapon_apple->attack_cd_timer / t;
            k_sprite_renderer_scale_x(weapon_apple->spr_rdr, scale);
            k_sprite_renderer_scale_y(weapon_apple->spr_rdr, scale);
        }
    }
}

struct yx_obj_rival_weapon *yx_obj_rival_weapon_apple_create(void) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_rival_weapon_apple));
    if (NULL == object)
        goto err;

    struct yx_obj_rival_weapon_apple *weapon_apple = k_object_get_data(object);
    weapon_apple->super.object = object;
    weapon_apple->super.v_tbl = &yx__obj_rival_weapon_apple_v_tbl;

    weapon_apple->x = 0;
    weapon_apple->y = 0;
    weapon_apple->aim_x = 0;
    weapon_apple->aim_y = 0;
    weapon_apple->attack_cd_time = 0.9f;

    {
        struct k_sprite_renderer_config config;
        config.x       = &weapon_apple->x;
        config.y       = &weapon_apple->y;
        config.sprite  = yx_spr_weapon_apple;
        config.z_group = YX_CONFIG_Z_GROUP_WEAPON;
        config.z_layer = 0;
        weapon_apple->spr_rdr = k_object_add_sprite_renderer(object, &config);
        if (NULL == weapon_apple->spr_rdr)
            goto err;
    }

    if (NULL == k_object_add_step_callback(object, yx__obj_rival_weapon_apple_on_step))
        goto err;

    return &weapon_apple->super;

err:
    k_log_error("failed to create object: yx_obj_rival_weapon_apple");
    return NULL;
}

/* endregion */

/* endregion */
