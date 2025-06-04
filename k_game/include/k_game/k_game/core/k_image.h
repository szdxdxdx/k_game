#ifndef K_IMAGE_H
#define K_IMAGE_H

#include <stdint.h>

#include "./k_game_fwd.h"

/**
 * \brief 加载一张图片
 *
 * 目前支持加载 .png 和 .jpg 格式的图片。
 *
 * 若加载成功，函数返回图片的指针，否则返回 `NULL`。
 */
struct k_image *k_image_load(const char *file_path);

/**
 * \brief 释放图片资源
 *
 * 游戏结束时 k_game 会释放图片资源，无需手动释放。
 */
void k_image_release(struct k_image *image);

/** \brief 获取图片的宽高 */
int k_image_get_w(struct k_image *image);
int k_image_get_h(struct k_image *image);

/**
 * \brief 拉伸图片
 *
 * 将图片 `image` 拉伸至指定宽高。
 *
 * 函数返回拉伸后的图片指针，若失败则返回 `NULL`。
 */
struct k_image *k_image_scale(struct k_image *image, int scaled_w, int scaled_h);

#endif
