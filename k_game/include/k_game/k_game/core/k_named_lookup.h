#ifndef K_NAMED_LOOKUP_H
#define K_NAMED_LOOKUP_H

#include "k_game_fwd.h"

/**
 * k_game 提供对资源的命名和查找机制。
 *
 * 在 k_game 中，资源一旦被加载或创建成功，则函数返回其指针，之后直接通过该指针来操作资源。
 * 默认情况下，资源没有名称，可手动为其设置字符串的名称，之后能根据名称查找资源。
 *
 * 通过 `k_XXX_set_name()` 系列的函数给设置唯一名称，若名称设为空字符串 "" 或 `NULL`，则清除名称。
 * 若设置成功，函数返回 0，否则返回非 0。
 *
 * 通过 `k_XXX_find()` 系列的函数根据名称查找资源。
 * 若找到，函数返回资源指针，否则返回 `NULL`。
 *
 * @{
 */

int k_image_set_name(struct k_image *image, const char *image_name);
int k_sprite_set_name(struct k_sprite *sprite, const char *sprite_name);
int k_sound_bgm_set_name(struct k_sound_bgm *sound, const char *bgm_name);
int k_sound_sfx_set_name(struct k_sound_sfx *sound, const char *sfx_name);
int k_font_set_name(struct k_font *font, const char *font_name);
int k_room_set_name(struct k_room *room, const char *room_name);
int k_component_type_set_name(struct k_component_type *component_type, const char *type_name);

struct k_image *k_image_find(const char *image_name);
struct k_sprite *k_sprite_find(const char *sprite_name);
struct k_sound_bgm *k_sound_bgm_find(const char *bgm_name);
struct k_sound_sfx *k_sound_sfx_find(const char *sfx_name);
struct k_font *k_font_find(const char *font_name);
struct k_room *k_room_find(const char *room_name);
struct k_component_type *k_component_type_find(const char *type_name);

/** @} */

#endif
