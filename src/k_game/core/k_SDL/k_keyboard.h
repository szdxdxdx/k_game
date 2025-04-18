#ifndef K__KEYBOARD_H
#define K__KEYBOARD_H

#include "SDL_events.h"

#include "k_game/core/k_keyboard.h"

struct k_keyboard_context {

    /* 记录键盘按键状态
     *
     * 每个按键用 3 个 bit 记录状态：
     * - 0b100 表示该按键在上一帧被按下或按住
     * - 0b010 表示该按键在这一帧被按下
     * - 0b001 表示该按键在这一帧抬起
     */
    uint8_t key_state[K_KEY_ENUM_END + 1];
};

extern struct k_keyboard_context k__keyboard;

void k__keyboard_refresh_state(void);

void k__SDL_handle_event_key_down(SDL_KeyboardEvent *event);

void k__SDL_handle_event_key_up(SDL_KeyboardEvent *event);

#endif
