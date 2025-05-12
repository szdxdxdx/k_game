
#define K_LOG_TAG "yx:room:room_1" /* 定义日志输出的标签 */

#include <math.h>
#include "k_log.h"

#include "k_game.h"
#include "./yx_room_1.h"

#if 0 /* 介绍如何创建房间 */

static void yx_room_1_on_step(void *data) { /* 房间运行时，每帧调用是步事件回调 */
    k_log_info("room_1 on step");
}
static int yx_room_1_on_create(void *param) { /* 创建房间时执行的回调，在此完成房间的初始化 */
    k_log_info("room_1 on create");
    k_room_add_step_callback(NULL, yx_room_1_on_step); /* 给房间添加一个步事件回调 */
    return 0;
}
static void yx_room_1_on_enter(void) { /* 进入房间时执行的回调 */
    k_log_info("room_1 on enter");
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT; /* 默认的初始化配置，可快速填充字段 */
    config.room_w    = 1920; /* 指定房间的宽高 */
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1); /* 指定房间关联数据的结构体大小 */
    config.on_create = yx_room_1_on_create; /* 指定房间的回调 */
    config.on_enter  = yx_room_1_on_enter;
    struct k_room *room = k_room_create(&config, NULL); /* 创建房间 */
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 0 /* 介绍按键检测 */

static void yx_room_1_on_step(void *data) {
    if (k_key_down('A')) k_log_info("按键按下");
    if (k_key_up('A'))   k_log_info("按键抬起");
    if (k_key_held('A')) k_log_info("按键按住");
}

static int yx_room_1_on_create(void *param) {
    k_room_add_step_callback(NULL, yx_room_1_on_step);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 0 /* 介绍画布，绘制房间背景 */

static void yx_room_1_on_draw_grid(void *data) {
    k_canvas_set_draw_color(0x3d3d3dff); /* 设置画笔颜色 */
    k_canvas_room_clear(); /* 清空画布 */

    float view_x, view_y, view_w, view_h;  /* 获取视野矩形 */
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);
    k_canvas_set_draw_color(0xaaaaaaff); /* 设置画笔颜色 */
    float grid_size = 48.0f;
    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        /* 绘制竖直的网格线 */
        k_canvas_room_draw_line(x, 0, x, view_h);
        x += grid_size;
    }
    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        /* 绘制水平的网格线 */
        k_canvas_room_draw_line(0, y, view_w, y);
        y += grid_size;
    }
}

static int yx_room_1_on_create(void *param) {
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_grid, 0, 0); /* 添加房间的绘制事件回调，绘制深度为 (0, 0) */
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif


#if 0 /* 介绍画布，绘制 UI */

static void yx_room_1_on_draw_grid(void *data) {
    k_canvas_set_draw_color(0x1e1e1eff); /* 设置画笔颜色 */
    k_canvas_room_clear(); /* 清空画布 */

    float view_x, view_y, view_w, view_h;  /* 获取视野矩形 */
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);
    k_canvas_set_draw_color(0x323333ff); /* 设置画笔颜色 */
    float grid_size = 48.0f;
    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        /* 绘制竖直的网格线 */
        k_canvas_room_draw_line(x, 0, x, view_h);
        x += grid_size;
    }
    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        /* 绘制水平的网格线 */
        k_canvas_room_draw_line(0, y, view_w, y);
        y += grid_size;
    }
}

static void yx_room_1_on_draw_ui_mouse_xy(void *data) {

    k_canvas_set_draw_color(0x00000000); /* 设置画笔颜色 */
    k_canvas_ui_clear(); /* 清空 UI 画布 */

    k_canvas_set_draw_color(0xffffffff); /* 设置画笔颜色 */
    float x = k_mouse_x();/* 获取鼠标在房间中的位置，并输出到 UI */
    float y = k_mouse_y();
    k_canvas_ui_printf(NULL, 8, 8, "(%.2f, %.2f)", x, y);
}

static int yx_room_1_on_create(void *param) {
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_ui_mouse_xy, 0, 0);
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_grid, 0, 0);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 0 /* 介绍对象 */

static void yx_room_1_on_draw_grid(void *data) {
    k_canvas_set_draw_color(0x1e1e1eff); /* 设置画笔颜色 */
    k_canvas_room_clear(); /* 清空画布 */

    float view_x, view_y, view_w, view_h;  /* 获取视野矩形 */
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);
    k_canvas_set_draw_color(0x323333ff); /* 设置画笔颜色 */
    float grid_size = 48.0f;
    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        /* 绘制竖直的网格线 */
        k_canvas_room_draw_line(x, 0, x, view_h);
        x += grid_size;
    }
    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        /* 绘制水平的网格线 */
        k_canvas_room_draw_line(0, y, view_w, y);
        y += grid_size;
    }
}

static void yx_room_1_on_draw_ui_mouse_xy(void *data) {

    k_canvas_set_draw_color(0x00000000); /* 设置画笔颜色 */
    k_canvas_ui_clear(); /* 清空 UI 画布 */

    k_canvas_set_draw_color(0xffffffff); /* 设置画笔颜色 */
    float x = k_mouse_x();/* 获取鼠标在房间中的位置，并输出到 UI */
    float y = k_mouse_y();
    k_canvas_ui_printf(NULL, 8, 8, "(%.2f, %.2f)", x, y);
}


struct yx_obj_player { float x, y; }; /* 定义一个玩家对象，它有一对坐标 (x, y) */

static void yx_obj_player_on_step(struct k_object *obj_player) { /* 玩家对象的步事件，用 WASD 控制移动 */
    struct yx_obj_player *player = k_object_get_data(obj_player); /* 获取对象数据 */
    float delta = k_time_get_step_delta(); /* 获取游戏帧间隔的事件 */
    if (k_key_held('A')) player->x -= 100.0f * delta; /* 检测按键，移动对象 */
    if (k_key_held('D')) player->x += 100.0f * delta;
    if (k_key_held('W')) player->y -= 100.0f * delta;
    if (k_key_held('S')) player->y += 100.0f * delta;
}

static void yx_obj_player_on_draw(struct k_object *obj_player) { /* 玩家的绘制事件，在自身坐标处绘制一个圆 */
    struct yx_obj_player *player = k_object_get_data(obj_player);
    k_canvas_set_draw_color(0xff6600ff);
    k_canvas_room_draw_circle(player->x, player->y, 20);
}

struct k_object *yx_obj_player_create(float x, float y) { /* 创建玩家对象 */
    struct k_object *obj_player = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == obj_player) return NULL;
    struct yx_obj_player *player = k_object_get_data(obj_player);
    player->x = x; player->y = y; /* 设置玩家对象的属性 */
    k_object_add_step_callback(obj_player, yx_obj_player_on_step); /* 添加事件回调 */
    k_object_add_draw_callback(obj_player, yx_obj_player_on_draw, 0, 1);
    return obj_player;
}

static int yx_room_1_on_create(void *param) {
    yx_obj_player_create(20, 20); /* 在当前房间的 (20, 20) 处添加一个玩家对象 */
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_ui_mouse_xy, 0, 0);
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_grid, 0, 0);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 0 /* 介绍组件 */

/* region [room_draw] */

static void yx_room_1_on_draw_grid(void *data) {
    k_canvas_set_draw_color(0x1e1e1eff); /* 设置画笔颜色 */
    k_canvas_room_clear(); /* 清空画布 */

    float view_x, view_y, view_w, view_h;  /* 获取视野矩形 */
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);
    k_canvas_set_draw_color(0x323333ff); /* 设置画笔颜色 */
    float grid_size = 48.0f;
    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        /* 绘制竖直的网格线 */
        k_canvas_room_draw_line(x, 0, x, view_h);
        x += grid_size;
    }
    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        /* 绘制水平的网格线 */
        k_canvas_room_draw_line(0, y, view_w, y);
        y += grid_size;
    }
}

static void yx_room_1_on_draw_ui_mouse_xy(void *data) {

    k_canvas_set_draw_color(0x00000000); /* 设置画笔颜色 */
    k_canvas_ui_clear(); /* 清空 UI 画布 */

    k_canvas_set_draw_color(0xffffffff); /* 设置画笔颜色 */
    float x = k_mouse_x();/* 获取鼠标在房间中的位置，并输出到 UI */
    float y = k_mouse_y();
    k_canvas_ui_printf(NULL, 8, 8, "(%.2f, %.2f)", x, y);
}

/* endregion */

/* 自定义组件，用于通过 WASD 移动对象，该组件通过指针关联一对外部坐标 */
struct yx_comp_wasd { float *x, *y; };

/* 组件的步事件，通过 WASD 移动坐标 */
static void yx_comp_wasd_on_step(struct k_component *component) {
    struct yx_comp_wasd *wasd = k_component_get_data(component); /* 获取组件的自定义数据 */
    float delta = k_time_get_step_delta(); /* 获取游戏帧间隔的事件 */
    if (k_key_held('A')) (*wasd->x) -= 100.0f * delta; /* 检测按键，设置关联坐标的值 */
    if (k_key_held('D')) (*wasd->x) += 100.0f * delta;
    if (k_key_held('W')) (*wasd->y) -= 100.0f * delta;
    if (k_key_held('S')) (*wasd->y) += 100.0f * delta;
}
/* 创建组件时执行的回调 */
static int yx_comp_wasd_on_create(struct k_component *component, void *param) {
    struct yx_comp_wasd *wasd = k_component_get_data(component); /* 获取组件的自定义数据 */
    float **xy = param; /* 获取给对象添加组件时传递的参数（这里因为自己知道传了什么参数，所以直接转换指针） */
    wasd->x = xy[0]; wasd->y = xy[1]; /* 设置组件的关联坐标 */
    k_component_add_step_callback(component, yx_comp_wasd_on_step); /* 给组件添加步事件回调 */
    return 0;
}
/* 注册 WASD 组件类型（该函数需要在游戏初始化时调用，然后才能给对象添加组件） */
static void yx_comp_wasd_type_register(void) {
    struct  k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT; /* 定义组件实例的行为 */
    entity_config.data_size = sizeof(struct yx_comp_wasd); /* 指定组件的用户自定义数据结构体大小 */
    entity_config.on_create = yx_comp_wasd_on_create; /* 指定创建组件时执行的回调 */
    struct k_component_manager_config *manager_config = NULL; /* 该组件不需要组件管理器，所以不需要定义管理器的行为 */
    struct k_component_type *type = k_component_type_register(manager_config, &entity_config); /* 注册组件类型 */
    k_component_type_set_name(type, "yx:wasd"); /* 给这个组件类型起个名字 */
}


struct yx_obj_player { float x, y; }; /* 定义一个玩家对象，它有一对坐标 (x, y) */

static void yx_obj_player_on_draw(struct k_object *obj_player) { /* 玩家的绘制事件，在自身坐标处绘制一个圆 */
    struct yx_obj_player *player = k_object_get_data(obj_player);
    k_canvas_set_draw_color(0xff6600ff);
    k_canvas_room_draw_circle(player->x, player->y, 20);
}

struct k_object *yx_obj_player_create(float x, float y) { /* 创建玩家对象 */
    struct k_object *obj_player = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == obj_player)
        return NULL;
    struct yx_obj_player *player = k_object_get_data(obj_player);
    player->x = x; player->y = y;

    struct k_component_type *wasd = k_component_type_find("yx:wasd"); /* 根据名字查找 wasd 的组件类型 */
    float *xy[2] = { &player->x, &player->y }; /* 绑定 wasd 组件时需要的入参，就是对象自身的坐标 */
    k_object_add_component(obj_player, wasd, xy); /* 给对象绑定组件 */

    k_object_add_draw_callback(obj_player, yx_obj_player_on_draw, 0, 1);
    return obj_player;
}

static int yx_room_1_on_create(void *param) {
    yx_comp_wasd_type_register();
    yx_obj_player_create(20, 20); /* 在当前房间的 (20, 20) 处添加一个玩家对象 */
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_ui_mouse_xy, 0, 0);
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_grid, 0, 0);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif

#if 1 /* 介绍组件 */

/* region [room_draw] */

static void yx_room_1_on_draw_grid(void *data) {
    k_canvas_set_draw_color(0x1e1e1eff); /* 设置画笔颜色 */
    k_canvas_room_clear(); /* 清空画布 */

    float view_x, view_y, view_w, view_h;  /* 获取视野矩形 */
    k_view_get_rect(&view_x, &view_y, &view_w, &view_h);
    k_canvas_set_draw_color(0x323333ff); /* 设置画笔颜色 */
    float grid_size = 48.0f;
    float x = floorf(view_x / grid_size) * grid_size;
    float x_to = ceilf((view_x + view_w) / grid_size) * grid_size;
    while (x < x_to) {
        /* 绘制竖直的网格线 */
        k_canvas_room_draw_line(x, 0, x, view_h);
        x += grid_size;
    }
    float y = floorf(view_y / grid_size) * grid_size;
    float y_to = ceilf((view_y + view_h) / grid_size) * grid_size;
    while (y < y_to) {
        /* 绘制水平的网格线 */
        k_canvas_room_draw_line(0, y, view_w, y);
        y += grid_size;
    }
}

static void yx_room_1_on_draw_ui_mouse_xy(void *data) {

    k_canvas_set_draw_color(0x00000000); /* 设置画笔颜色 */
    k_canvas_ui_clear(); /* 清空 UI 画布 */

    k_canvas_set_draw_color(0xffffffff); /* 设置画笔颜色 */
    float x = k_mouse_x();/* 获取鼠标在房间中的位置，并输出到 UI */
    float y = k_mouse_y();
    k_canvas_ui_printf(NULL, 8, 8, "(%.2f, %.2f)", x, y);
}

/* endregion */

/* region [load_sprite] */

static struct k_sprite *yx_spr_player_idle = NULL;

static struct k_image *yx_img_player_sprite_sheet = NULL; /* 玩家角色的精灵表 */
static struct k_sprite *yx_spr_player_run  = NULL; /* 玩家角色的跑步动画，这里将它设为全局变量，方便使用 */

/* 加载精灵素材（在初始化游戏时执行调用本函数） */
static void yx_load_sprite(void) {
    struct k_image *img = k_image_load("demo_1/sprite/ynx.png"); /* 加载原素材图片 */
    int img_w = k_image_get_w(img); /* 原素材图片尺寸太小，这里需要对它放大（获取宽高后乘以 2） */
    int img_h = k_image_get_h(img);
    yx_img_player_sprite_sheet = k_image_scale(img, img_w * 2, img_h * 2); /* 拉伸原素材图片，得到精灵表 */
    k_image_release(img); /* 释放原素材图片 */

    struct k_sprite_frame_config frames[6]; /* 玩家角色一共有 6 个精灵帧 */
    int i = 0;
    for (; i < 6; i++) {
        frames[i].offset_x = i * 64; /* 定义每个精灵帧在精灵表中的偏移 */
        frames[i].offset_y = 64;
        frames[i].image = yx_img_player_sprite_sheet;
        frames[i].delay = 100; /* 播放时每帧延时 100 毫秒 */
    }
    struct k_sprite_config sprite_config;
    sprite_config.sprite_w = 64; /* 精灵的宽高尺寸是 64x64，中心点设在 32x32 */
    sprite_config.sprite_h = 64;
    sprite_config.origin_x = 32;
    sprite_config.origin_y = 32;
    sprite_config.frames = frames;
    sprite_config.frames_num = 6;
    yx_spr_player_run = k_sprite_create(&sprite_config); /* 创建精灵 */
}

/* endregion */

/* region [wasd] */

/* 自定义组件，用于通过 WASD 移动对象，该组件通过指针关联一对外部坐标 */
struct yx_comp_wasd { float *x, *y; };

/* 组件的步事件，通过 WASD 移动坐标 */
static void yx_comp_wasd_on_step(struct k_component *component) {
    struct yx_comp_wasd *wasd = k_component_get_data(component); /* 获取组件的自定义数据 */
    float delta = k_time_get_step_delta(); /* 获取游戏帧间隔的事件 */
    if (k_key_held('A')) (*wasd->x) -= 100.0f * delta; /* 检测按键，设置关联坐标的值 */
    if (k_key_held('D')) (*wasd->x) += 100.0f * delta;
    if (k_key_held('W')) (*wasd->y) -= 100.0f * delta;
    if (k_key_held('S')) (*wasd->y) += 100.0f * delta;
}
/* 创建组件时执行的回调 */
static int yx_comp_wasd_on_create(struct k_component *component, void *param) {
    struct yx_comp_wasd *wasd = k_component_get_data(component); /* 获取组件的自定义数据 */
    float **xy = param; /* 获取给对象添加组件时传递的参数（这里因为自己知道传了什么参数，所以直接转换指针） */
    wasd->x = xy[0]; wasd->y = xy[1]; /* 设置组件的关联坐标 */
    k_component_add_step_callback(component, yx_comp_wasd_on_step); /* 给组件添加步事件回调 */
    return 0;
}
/* 注册 WASD 组件类型（该函数需要在游戏初始化时调用，然后才能给对象添加组件） */
static void yx_comp_wasd_type_register(void) {
    struct  k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT; /* 定义组件实例的行为 */
    entity_config.data_size = sizeof(struct yx_comp_wasd); /* 指定组件的用户自定义数据结构体大小 */
    entity_config.on_create = yx_comp_wasd_on_create; /* 指定创建组件时执行的回调 */
    struct k_component_manager_config *manager_config = NULL; /* 该组件不需要组件管理器，所以不需要定义管理器的行为 */
    struct k_component_type *type = k_component_type_register(manager_config, &entity_config); /* 注册组件类型 */
    k_component_type_set_name(type, "yx:wasd"); /* 给这个组件类型起个名字 */
}

/* endregion */

/* region [player] */

struct yx_obj_player { float x, y; }; /* 定义一个玩家对象，它有一对坐标 (x, y) */

static void yx_obj_player_on_draw(struct k_object *obj_player) { /* 玩家的绘制事件，在自身坐标处绘制一个圆 */
    struct yx_obj_player *player = k_object_get_data(obj_player);
    k_canvas_set_draw_color(0xff6600ff);
    k_canvas_room_draw_circle(player->x, player->y, 20);
}

struct k_object *yx_obj_player_create(float x, float y) { /* 创建玩家对象 */
    /* region {...} */
    struct k_object *obj_player = k_object_create(sizeof(struct yx_obj_player));
    if (NULL == obj_player)
        return NULL;
    struct yx_obj_player *player = k_object_get_data(obj_player);
    player->x = x; player->y = y;
    /* endregion */
    struct k_component_type *wasd = k_component_type_find("yx:wasd");
    float *xy[2] = { &player->x, &player->y };
    k_object_add_component(obj_player, wasd, xy); /* 给对象绑定 wasd 组件 */

    struct k_sprite_renderer_config spr_rdr; /* <- 精灵渲染器的配置 */
    spr_rdr.x       = &player->x; /* 指定渲染器关联的坐标 */
    spr_rdr.y       = &player->y;
    spr_rdr.sprite  = yx_spr_player_run; /* 指定使用的精灵资源 */
    spr_rdr.z_group = 0; /* 指定绘图深度为(0, 2) */
    spr_rdr.z_layer = 2;
    k_object_add_sprite_renderer(obj_player, &spr_rdr); /* 给玩家对象添加精灵渲染器组件 */

    k_object_add_draw_callback(obj_player, yx_obj_player_on_draw, 0, 1);
    return obj_player;
}

/* endregion */

static int yx_room_1_on_create(void *param) {

    yx_comp_wasd_type_register();
    yx_load_sprite();

    yx_obj_player_create(20, 20); /* 在当前房间的 (20, 20) 处添加一个玩家对象 */
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_ui_mouse_xy, 0, 0);
    k_room_add_draw_callback(NULL, yx_room_1_on_draw_grid, 0, 0);
    return 0;
}

struct k_room *yx_room_1_create(void) {
    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.room_w    = 1920;
    config.room_h    = 1080;
    config.data_size = sizeof(struct yx_room_1);
    config.on_create = yx_room_1_on_create;
    struct k_room *room = k_room_create(&config, NULL);
    if (NULL == room) {
        k_log_error("failed to create room_1");
        return NULL;
    }
    return room;
}

#endif