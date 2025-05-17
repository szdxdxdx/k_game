
#define K_LOG_TAG "yx:object:player"
#include "k_log.h"

#include "k_game.h"

#include "object/player/yx_obj_player.h"

static void yx__obj_player_on_debug_step_spr_rdr(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    float delta = k_time_get_step_delta();

    /* 旋转 */
    if (k_key_down_or_held('R'))
        k_sprite_renderer_rotate(player->spr_rdr, 180.0f * delta + k_sprite_renderer_get_rotation(player->spr_rdr));

    /* 翻转 */
    if (k_key_down_or_held('F')) {
        if (k_key_down('X'))
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        if (k_key_down('Y'))
            k_sprite_renderer_flip_y(player->spr_rdr, 1);
        if (k_key_down('C'))
            k_sprite_renderer_flip_y(player->spr_rdr, 0);
    }

    /* 放缩 */
    if (k_key_down_or_held('Z')) {
        if (k_key_down_or_held('X'))
            k_sprite_renderer_set_scaled_w(player->spr_rdr, 6 + k_sprite_renderer_get_w(player->spr_rdr));
        if (k_key_down_or_held('Y'))
            k_sprite_renderer_set_scaled_h(player->spr_rdr, 6 + k_sprite_renderer_get_h(player->spr_rdr));
    }

    /* 加速减速 */
    if (k_key_down_or_held('='))
        k_sprite_renderer_set_speed(player->spr_rdr, 1.0f * delta + k_sprite_renderer_get_speed(player->spr_rdr));
    if (k_key_down_or_held('-'))
        k_sprite_renderer_set_speed(player->spr_rdr, -0.5f * delta + k_sprite_renderer_get_speed(player->spr_rdr));

    if (k_key_down('B')) {
        if (k_key_held(K_KEY_LEFT_SHIFT))
            k_sprite_renderer_set_debug(player->spr_rdr, 0);
        else
            k_sprite_renderer_set_debug(player->spr_rdr, 1);
    }

    if (k_key_down('H')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, NULL);
    }
    if (k_key_down('G')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);
    }

    if (k_key_down('V')) {
        k_view_set_position(player->x + 2.0f, player->y + 2.0f);
    }
}

static void yx__obj_player_on_debug_draw_position(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    k_canvas_set_draw_color(0xffffffff);
    k_canvas_ui_printf(NULL, 8, 56, "(%.2f, %.2f)", player->x, player->y);
}

int yx__obj_player_on_create_add_debug(struct yx_obj_player *player) {
    // k_object_add_step_callback(player->object, yx__obj_player_on_debug_step_spr_rdr);
    // k_object_add_draw_callback(player->object, yx__obj_player_on_debug_draw_position, 0, 0);
    return 0;
}
