#ifndef K_GAME_FWD_H
#define K_GAME_FWD_H

/**
 * \brief 图片
 *
 */
struct k_image;

struct k_font;

struct k_sound_bgm;

struct k_sound_sfx;

/**
 * \brief 精灵
 *
 * 【精灵】可以是一张静态图片，或是一组连续的动画帧，
 * 用来表示游戏中的各种可见元素，例如：角色、敌人、道具等。
 */
struct k_sprite;

/**
 * \brief 房间
 *
 * 【房间】是游戏运行的场所。
 */
struct k_room;

/**
 * \brief 对象
 *
 * 【对象】是游戏中会发生动作的实体，它们有行为，例如：移动、攻击等。
 * 大多数情况下，【对象】使用【精灵】作为自己的外观表现，使得它们能被看见。
 * 所以你在游戏中看到的绝大多数东西都是【对象】，例如：主角、子弹、墙壁等。
 * 当然也有一些【对象】没有外观，它们仅用于控制游戏的运作。
 */
struct k_object;

struct k_component;

struct k_component_type;

struct k_component_manager;

struct k_callback;

/** \brief 表示一个点坐标 */
struct k_float_point {
    float x;
    float y;
};

/** \brief 表示一个矩形结构，`(x, y)` 为矩形左上角的坐标，`w` 和 `h` 为宽和高 */
struct k_int_rect {
    int x;
    int y;
    int w;
    int h;
};

/** \brief 表示一个矩形结构，`(x, y)` 为矩形左上角的坐标，`w` 和 `h` 为宽和高 */
struct k_float_rect {
    float x;
    float y;
    float w;
    float h;
};

#endif
