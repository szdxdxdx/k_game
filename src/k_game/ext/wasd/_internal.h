#ifndef K_GAME__WASD__INTERNAL_H
#define K_GAME__WASD__INTERNAL_H

#include "k_game.h"

#include "./_public.h"

struct k_WASD {

    float *x;
    float *y;

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_right;

    float speed;
};

#endif
