#ifndef K_WASD_H
#define K_WASD_H

#include "k_game/core/k_keyboard.h"

/* 已弃用 */

struct k_WASD_config {

    float *x;
    float *y;

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_right;

    float speed;
};

#endif
