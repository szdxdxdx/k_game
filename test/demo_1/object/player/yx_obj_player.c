#include "../_internal.h"

static void player_step_set_state(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    struct k_float_vec2 *position = &player->position;
    struct k_float_vec2 *next_position = &player->position_next;

    enum yx_obj_player_state state = YX_OBJ_PLAYER_STATE_IDLE;
    if (next_position->x < position->x) {
        state = YX_OBJ_PLAYER_STATE_RUN;
        player->face = -1;
    }
    else if (next_position->x > position->x) {
        state = YX_OBJ_PLAYER_STATE_RUN;
        player->face = 1;
    }

    if (next_position->y != position->y) {
        state = YX_OBJ_PLAYER_STATE_RUN;
        k_sprite_renderer_set_z_layer(player->spr_rdr, (int)next_position->y);
        yx_obj_weapon_set_z_index(player->weapon, (int)next_position->y + 1);
    }

    switch (state) {
        case YX_OBJ_PLAYER_STATE_RUN: {

            switch (player->state) {
                case YX_OBJ_PLAYER_STATE_RUN:
                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                    else
                        k_sprite_renderer_flip_x(player->spr_rdr, 0);
                    break;
                case YX_OBJ_PLAYER_STATE_IDLE:
                    player->state = YX_OBJ_PLAYER_STATE_RUN;
                    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);
                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                    break;
            }
            break;
        }
        case YX_OBJ_PLAYER_STATE_IDLE: {

            switch (player->state) {
                case YX_OBJ_PLAYER_STATE_RUN:player->state = YX_OBJ_PLAYER_STATE_IDLE;
                    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_idle);

                    if (player->face == 1)
                        k_sprite_renderer_flip_x(player->spr_rdr, 1);
                case YX_OBJ_PLAYER_STATE_IDLE:
                    break;
            }
            break;
        }
    }

    position->x = next_position->x;
    position->y = next_position->y;
}

static void player_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    float delta = k_get_step_delta();

    /* 旋转 */
    if (k_key_down('R'))
        k_sprite_renderer_rotate(player->spr_rdr, 180.0f * delta + k_sprite_renderer_get_rotation(player->spr_rdr));

    /* 翻转 */
    if (k_key_down('F')) {
        if (k_key_pressed('X'))
            k_sprite_renderer_flip_x(player->spr_rdr, 1);
        if (k_key_pressed('Y'))
            k_sprite_renderer_flip_y(player->spr_rdr, 1);
        if (k_key_pressed('C'))
            k_sprite_renderer_flip_y(player->spr_rdr, 0);
    }

    /* 放缩 */
    if (k_key_down('Z')) {
        if (k_key_down('X'))
            k_sprite_renderer_set_w(player->spr_rdr, 6 + k_sprite_renderer_get_w(player->spr_rdr));
        if (k_key_down('Y'))
            k_sprite_renderer_set_h(player->spr_rdr, 6 + k_sprite_renderer_get_h(player->spr_rdr));
    }

    /* 加速减速 */
    if (k_key_down('='))
        k_sprite_renderer_set_speed(player->spr_rdr, 1.0f * delta + k_sprite_renderer_get_speed(player->spr_rdr));
    if (k_key_down('-'))
        k_sprite_renderer_set_speed(player->spr_rdr, -0.5f * delta + k_sprite_renderer_get_speed(player->spr_rdr));

    if (k_key_pressed('B')) {
        if (k_key_held(K_KEY_LEFT_SHIFT))
            k_sprite_renderer_set_debug(player->spr_rdr, 0);
        else
            k_sprite_renderer_set_debug(player->spr_rdr, 1);
    }

    if (k_key_pressed('H')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, NULL);
    }
    if (k_key_pressed('G')) {
        k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);
    }
}

static void player_touch_bubble(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    float padding = 6;
    float x1 = player->position.x - padding;
    float y1 = player->position.y - padding;
    float x2 = player->position.x + padding;
    float y2 = player->position.y + padding;
    struct k_collision_box *box = k_collision_check_rectangle(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    if (NULL != box)
        yx_bubble_pop(k_collision_box_get_object(box));
}

struct k_object *yx_player_create(const struct yx_obj_player_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));

    k_object_add_step_begin_callback(object, player_step_set_state);
    k_object_add_step_callback(object, player_touch_bubble);
    //k_object_add_step_callback(object, player_step);
    //k_object_add_step_callback(object, player_step_shoot);

    struct yx_obj_player *player = k_object_get_data(object);
    player->position.x      = config->x;
    player->position.y      = config->y;
    player->position_next.x = config->x;
    player->position_next.y = config->y;
    player->state           = YX_OBJ_PLAYER_STATE_IDLE;
    player->face            = -1;

    player->spr_idle = config->spr_idle;
    player->spr_run  = config->spr_run;

    {
        struct k_component_type *WASD = k_component_type_find("k/WASD");
        struct k_WASD_config WASD_config;
        WASD_config.key_up    = 'W';
        WASD_config.key_left  = 'A';
        WASD_config.key_down  = 'S';
        WASD_config.key_right = 'D';
        WASD_config.speed     = 192.0f;
        WASD_config.position = &player->position_next;
        player->WASD = k_object_add_component(object, WASD, &WASD_config);
    }

    {
        struct k_sprite_renderer_config renderer_config;
        renderer_config.position = &player->position;
        renderer_config.sprite   = player->spr_idle;
        renderer_config.z_group  = 0;
        renderer_config.z_layer  = (int)config->y;
        player->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);
    }

    {
        struct yx_obj_weapon_config weapon_config;
        weapon_config.position = &player->position;
        player->weapon = yx_obj_weapon_create(&weapon_config);
    }

    return object;
}
