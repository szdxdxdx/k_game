#ifndef K_GAME_WASD_H
#define K_GAME_WASD_H

struct k_float_vec2;
enum k_keyboard_key;

struct k_WASD_config {

    struct k_float_vec2 *position;

    enum k_keyboard_key key_up;
    enum k_keyboard_key key_left;
    enum k_keyboard_key key_down;
    enum k_keyboard_key key_right;

    float speed;
};

#endif
