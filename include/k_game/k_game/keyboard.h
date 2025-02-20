#ifndef K_GAME_KEYBOARD_H
#define K_GAME_KEYBOARD_H

enum k_keyboard_key;

int k_is_key_pressed(enum k_keyboard_key key);

int k_is_key_held(enum k_keyboard_key key);

int k_is_key_released(enum k_keyboard_key key);

int k_is_key_idle(enum k_keyboard_key key);

enum k_keyboard_key {
    K_KEY_A,
    K_KEY_B,
    K_KEY_C,
};

#endif
