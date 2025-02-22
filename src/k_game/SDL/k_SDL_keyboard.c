#include "./k_SDL_keyboard.h"

/* 将 SDL 的键码转为 k_game 的键码
 */
static enum k_keyboard_key SDL_key_to_k_key(SDL_Keycode SDL_key) {

    /* 请相信编译器的优化能力
     *
     * SDL 定义的键码虽不是完全连续，但也是大段连续的。
     * 下述的 switch-case 不会退化为冗长的 if-else。
     */
    switch (SDL_key) {

        case SDLK_BACKSPACE    : return K_KEY_BACKSPACE     ;
        case SDLK_TAB          : return K_KEY_TAB           ;
        case SDLK_RETURN       : return K_KEY_RETURN        ;
        case SDLK_ESCAPE       : return K_KEY_ESCAPE        ;
        case SDLK_SPACE        : return K_KEY_SPACE         ;

        case SDLK_EXCLAIM      : return K_KEY_EXCLAIM       ;
        case SDLK_QUOTEDBL     : return K_KEY_QUOTE_DBL     ;
        case SDLK_HASH         : return K_KEY_HASH          ;
        case SDLK_DOLLAR       : return K_KEY_DOLLAR        ;
        case SDLK_PERCENT      : return K_KEY_PERCENT       ;
        case SDLK_AMPERSAND    : return K_KEY_AMPERSAND     ;
        case SDLK_QUOTE        : return K_KEY_QUOTE         ;
        case SDLK_LEFTPAREN    : return K_KEY_LEFT_PAREN    ;
        case SDLK_RIGHTPAREN   : return K_KEY_RIGHT_PAREN   ;
        case SDLK_ASTERISK     : return K_KEY_ASTERISK      ;
        case SDLK_PLUS         : return K_KEY_PLUS          ;
        case SDLK_COMMA        : return K_KEY_COMMA         ;
        case SDLK_MINUS        : return K_KEY_MINUS         ;
        case SDLK_PERIOD       : return K_KEY_PERIOD        ;
        case SDLK_SLASH        : return K_KEY_SLASH         ;

        case SDLK_0            : return K_KEY_0             ;
        case SDLK_1            : return K_KEY_1             ;
        case SDLK_2            : return K_KEY_2             ;
        case SDLK_3            : return K_KEY_3             ;
        case SDLK_4            : return K_KEY_4             ;
        case SDLK_5            : return K_KEY_5             ;
        case SDLK_6            : return K_KEY_6             ;
        case SDLK_7            : return K_KEY_7             ;
        case SDLK_8            : return K_KEY_8             ;
        case SDLK_9            : return K_KEY_9             ;

        case SDLK_COLON        : return K_KEY_COLON         ;
        case SDLK_SEMICOLON    : return K_KEY_SEMICOLON     ;
        case SDLK_LESS         : return K_KEY_LESS          ;
        case SDLK_EQUALS       : return K_KEY_EQUALS        ;
        case SDLK_GREATER      : return K_KEY_GREATER       ;
        case SDLK_QUESTION     : return K_KEY_QUESTION      ;
        case SDLK_AT           : return K_KEY_AT            ;

        case SDLK_LEFTBRACKET  : return K_KEY_LEFT_BRACKET  ;
        case SDLK_BACKSLASH    : return K_KEY_BACKSLASH     ;
        case SDLK_RIGHTBRACKET : return K_KEY_RIGHT_BRACKET ;
        case SDLK_CARET        : return K_KEY_CARET         ;
        case SDLK_UNDERSCORE   : return K_KEY_UNDERSCORE    ;
        case SDLK_BACKQUOTE    : return K_KEY_BACK_QUOTE    ;

        case SDLK_a            : return K_KEY_A             ;
        case SDLK_b            : return K_KEY_B             ;
        case SDLK_c            : return K_KEY_C             ;
        case SDLK_d            : return K_KEY_D             ;
        case SDLK_e            : return K_KEY_E             ;
        case SDLK_f            : return K_KEY_F             ;
        case SDLK_g            : return K_KEY_G             ;
        case SDLK_h            : return K_KEY_H             ;
        case SDLK_i            : return K_KEY_I             ;
        case SDLK_j            : return K_KEY_J             ;
        case SDLK_k            : return K_KEY_K             ;
        case SDLK_l            : return K_KEY_L             ;
        case SDLK_m            : return K_KEY_M             ;
        case SDLK_n            : return K_KEY_N             ;
        case SDLK_o            : return K_KEY_O             ;
        case SDLK_p            : return K_KEY_P             ;
        case SDLK_q            : return K_KEY_Q             ;
        case SDLK_r            : return K_KEY_R             ;
        case SDLK_s            : return K_KEY_S             ;
        case SDLK_t            : return K_KEY_T             ;
        case SDLK_u            : return K_KEY_U             ;
        case SDLK_v            : return K_KEY_V             ;
        case SDLK_w            : return K_KEY_W             ;
        case SDLK_x            : return K_KEY_X             ;
        case SDLK_y            : return K_KEY_Y             ;
        case SDLK_z            : return K_KEY_Z             ;

        case SDLK_CAPSLOCK     : return K_KEY_CAPSLOCK      ;

        case SDLK_F1           : return K_KEY_F1            ;
        case SDLK_F2           : return K_KEY_F2            ;
        case SDLK_F3           : return K_KEY_F3            ;
        case SDLK_F4           : return K_KEY_F4            ;
        case SDLK_F5           : return K_KEY_F5            ;
        case SDLK_F6           : return K_KEY_F6            ;
        case SDLK_F7           : return K_KEY_F7            ;
        case SDLK_F8           : return K_KEY_F8            ;
        case SDLK_F9           : return K_KEY_F9            ;
        case SDLK_F10          : return K_KEY_F10           ;
        case SDLK_F11          : return K_KEY_F11           ;
        case SDLK_F12          : return K_KEY_F12           ;

        case SDLK_PRINTSCREEN  : return K_KEY_PRINT_SCREEN  ;
        case SDLK_SCROLLLOCK   : goto   k_key_no_support    ;
        case SDLK_PAUSE        : goto   k_key_no_support    ;
        case SDLK_INSERT       : goto   k_key_no_support    ;
        case SDLK_HOME         : goto   k_key_no_support    ;
        case SDLK_PAGEUP       : goto   k_key_no_support    ;
        case SDLK_DELETE       : return K_KEY_DELETE        ;
        case SDLK_END          : goto   k_key_no_support    ;
        case SDLK_PAGEDOWN     : goto   k_key_no_support    ;

        case SDLK_RIGHT        : return K_KEY_RIGHT         ;
        case SDLK_LEFT         : return K_KEY_LEFT          ;
        case SDLK_DOWN         : return K_KEY_DOWN          ;
        case SDLK_UP           : return K_KEY_UP            ;

        case SDLK_NUMLOCKCLEAR : return K_KEY_NUM_LOCK_CLEAR;
        case SDLK_KP_DIVIDE    : return K_KEY_KP_DIVIDE     ;
        case SDLK_KP_MULTIPLY  : return K_KEY_KP_MULTIPLY   ;
        case SDLK_KP_MINUS     : return K_KEY_KP_MINUS      ;
        case SDLK_KP_PLUS      : return K_KEY_KP_PLUS       ;
        case SDLK_KP_ENTER     : return K_KEY_KP_ENTER      ;
        case SDLK_KP_1         : return K_KEY_KP_1          ;
        case SDLK_KP_2         : return K_KEY_KP_2          ;
        case SDLK_KP_3         : return K_KEY_KP_3          ;
        case SDLK_KP_4         : return K_KEY_KP_4          ;
        case SDLK_KP_5         : return K_KEY_KP_5          ;
        case SDLK_KP_6         : return K_KEY_KP_6          ;
        case SDLK_KP_7         : return K_KEY_KP_7          ;
        case SDLK_KP_8         : return K_KEY_KP_8          ;
        case SDLK_KP_9         : return K_KEY_KP_9          ;
        case SDLK_KP_0         : return K_KEY_KP_0          ;
        case SDLK_KP_PERIOD    : return K_KEY_KP_PERIOD     ;

        case SDLK_LCTRL        : return K_KEY_LEFT_CTRL     ;
        case SDLK_LSHIFT       : return K_KEY_LEFT_SHIFT    ;
        case SDLK_LALT         : return K_KEY_LEFT_ALT      ;
        case SDLK_LGUI         : goto   k_key_no_support    ;
        case SDLK_RCTRL        : return K_KEY_RIGHT_CTRL    ;
        case SDLK_RSHIFT       : return K_KEY_RIGHT_SHIFT   ;
        case SDLK_RALT         : return K_KEY_RIGHT_ALT     ;
        case SDLK_RGUI         : goto   k_key_no_support    ;

        k_key_no_support:
        default: return K_KEY_ENUM_END;
    }
}

/* 记录键盘按键状态
 *
 * 每个按键的状态用 3 个 bit 位表示：
 * - 0b001 表示该按键在当前帧被按下
 * - 0b010 表示该按键在当前帧抬起
 * - 0b100 表示该按键在上一帧被按下或按住
 */
static unsigned int key_state[K_KEY_ENUM_END + 1];

void k__refresh_keyboard(void) {

    size_t i = 0;
    for (; i < K_KEY_ENUM_END; i++) {
        switch (key_state[i] & 0b11) {
            case 0b00: key_state[i] &= 0b100; break;
            case 0b01: key_state[i]  = 0b000; break;
            case 0b10: key_state[i]  = 0b100; break;
            case 0b11: key_state[i]  = 0b000; break;
        }
    }
}

void k__set_key_down(SDL_Keycode SDL_key) {
    enum k_keyboard_key k_key = SDL_key_to_k_key(SDL_key);
    key_state[k_key] |= 0b010;
}

void k__set_key_up(SDL_Keycode SDL_key) {
    enum k_keyboard_key k_key = SDL_key_to_k_key(SDL_key);
    key_state[k_key] |= 0b001;
}

int k_is_key_pressed(enum k_keyboard_key key) {
    return 0b010 == (key_state[key] & 0b110);
}

int k_is_key_released(enum k_keyboard_key key) {
    return 0b001 == (key_state[key] & 0b001);
}

int k_is_key_held(enum k_keyboard_key key) {
    return 0b100 == (key_state[key] & 0b101);
}

int k_is_key_idle(enum k_keyboard_key key) {
    return 0b000 == key_state[key];
}

int k_is_key_down(enum k_keyboard_key key) {
    return 0;
}

int k_is_key_up(enum k_keyboard_key key) {
    return 0; /* TODO */
}
