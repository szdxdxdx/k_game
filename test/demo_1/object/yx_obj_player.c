#include "./yx_obj.h"

/* region [state] */

/* 定义一个 idle 的状态节点，没有按键操作时，玩家对象处于该状态 */
static void yx_obj_player_on_idle_state_enter(struct k_object *object);
static void yx_obj_player_on_idle_state_step(struct k_object *object);
static struct k_state_machine_state STATE_IDLE = {
    yx_obj_player_on_idle_state_enter,
    yx_obj_player_on_idle_state_step,
    NULL
};

/* 定义一个 running 的状态节点，通过按键操控玩家移动时，玩家对象处于该状态 */
static void yx_obj_player_on_running_state_enter(struct k_object *object);
static void yx_obj_player_on_running_state_step(struct k_object *object);
static struct k_state_machine_state STATE_RUNNING = {
    yx_obj_player_on_running_state_enter,
    yx_obj_player_on_running_state_step,
    NULL
};

/* 当玩家处于 idle 状态时，每帧执行该回调 */
static void yx_obj_player_on_idle_state_step(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);
    /* 如果玩家所处的坐标发生了移动，则切换状态到 running，将一份新的状态节点交给状态机组件 */
    if (player->next_x != player->x || player->next_y != player->y) {
        k_state_machine_change_state(player->state_machine, STATE_RUNNING);
    }
}

static void yx_obj_player_on_idle_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    int flip_x = k_sprite_renderer_is_flipped_x(player->spr_rdr);

    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_idle);

    k_sprite_renderer_flip_x(player->spr_rdr, flip_x);
}

/* region [state_running] */

static void yx_obj_player_on_running_state_enter(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    int flip_x = k_sprite_renderer_is_flipped_x(player->spr_rdr);

    k_sprite_renderer_set_sprite(player->spr_rdr, player->spr_run);

    k_sprite_renderer_flip_x(player->spr_rdr, flip_x);
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

static void player_step(struct k_object *object) {
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
            k_sprite_renderer_set_w(player->spr_rdr, 6 + k_sprite_renderer_get_w(player->spr_rdr));
        if (k_key_down_or_held('Y'))
            k_sprite_renderer_set_h(player->spr_rdr, 6 + k_sprite_renderer_get_h(player->spr_rdr));
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

static void player_touch_bubble(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

#if 0
    float x1 = player->x;
    float y1 = player->y;
    float x2 = player->weapon->x;
    float y2 = player->weapon->y;
    struct k_collision_box *box = k_collision_check_rectangle(YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    if (NULL != box)
        yx_bubble_pop(k_collision_box_get_object(box));
#else

    float x1 = player->x;
    float y1 = player->y;
    float x2 = player->weapon->x;
    float y2 = player->weapon->y;

    struct k_collision_box *box[512];
    size_t n = k_collision_query_rect(box, 512, YX_COLLISION_GROUP_BUBBLE, x1, y1, x2, y2);
    size_t i = 0;
    for (; i < n; i++) {
        yx_obj_bubble_pop(k_collision_box_get_object(box[i]));
    }

#endif
}

static void player_draw_position(struct k_object *object) {
    struct yx_obj_player *player = k_object_get_data(object);

    k_canvas_set_draw_color(0xffffffff);
    k_canvas_ui_printf(NULL, 8, 56, "(%.2f, %.2f)", player->x, player->y);
}

struct k_object *yx_obj_player_create(const struct yx_obj_player_config *config) {

    struct k_object *object = k_object_create(sizeof(struct yx_obj_player));

    k_object_add_step_callback(object, player_touch_bubble);
    k_object_add_step_callback(object, player_step);

    k_object_add_draw_callback(object, player_draw_position, 0, 0);

    struct yx_obj_player *player = k_object_get_data(object);

    {
        struct k_position_config position_config;
        position_config.parent = NULL;
        position_config.world_x = &player->x;
        position_config.world_y = &player->y;
        position_config.local_x = config->x;
        position_config.local_y = config->y;
        player->position = k_object_add_position(object, &position_config);
    }

    {
        struct k_component_type *WASD = k_component_type_find("k/WASD");
        struct k_WASD_config WASD_config;
        WASD_config.key_up    = 'W';
        WASD_config.key_left  = 'A';
        WASD_config.key_down  = 'S';
        WASD_config.key_right = 'D';
        WASD_config.speed     = 192.0f;
        WASD_config.x         = &player->next_x;
        WASD_config.y         = &player->next_y;
        player->WASD = k_object_add_component(object, WASD, &WASD_config);

        player->next_x = config->x;
        player->next_y = config->y;
    }

    {
        player->spr_idle = config->spr_idle;
        player->spr_run  = config->spr_run;

        struct k_sprite_renderer_config renderer_config;
        renderer_config.x       = &player->x;
        renderer_config.y       = &player->y;
        renderer_config.sprite  = player->spr_idle;
        renderer_config.z_group = 0;
        renderer_config.z_layer = (int)config->y;
        player->spr_rdr = k_object_add_sprite_renderer(object, &renderer_config);
        k_sprite_renderer_flip_x(player->spr_rdr, 1);
    }

    {
        struct yx_obj_weapon_config weapon_config;
        weapon_config.parent = player->position;
        player->weapon = yx_obj_weapon_create(&weapon_config);
    }

    {
        player->state_machine = k_object_add_state_machine(object);
        k_state_machine_change_state(player->state_machine, STATE_IDLE);
    }

    {
        struct k_camera_target *target = k_camera_add_follow_target(object, &player->x, &player->y);
        k_camera_set_primary_target(target);
        k_camera_set_target_weight(target, 4.0f);
    }

    return object;
}
