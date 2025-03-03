#ifndef K_GAME_COMPONENT_WASD_H
#define K_GAME_COMPONENT_WASD_H

#include "k_game_keyboard.h"

struct k_WASD_config {

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_right;

    float speed;

    float *x, *y;
};

#endif
