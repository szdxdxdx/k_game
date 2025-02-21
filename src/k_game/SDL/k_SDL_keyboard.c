#include <stdio.h>
#include "./k_SDL_keyboard.h"

static unsigned int key_state[K_KEY_ENUM_END + 1];

static enum k_keyboard_key SDL_key_to_k_key(SDL_Keycode SDL_key) {

    switch (SDL_key) {

        case SDLK_LCTRL        : return K_KEY_LEFT_CTRL     ;
        case SDLK_LSHIFT       : return K_KEY_LEFT_SHIFT    ;
        case SDLK_LALT         : return K_KEY_LEFT_ALT      ;
        case SDLK_LGUI         : return K_KEY_RIGHT_CTRL    ;
        case SDLK_RCTRL        : return K_KEY_RIGHT_SHIFT   ;
        case SDLK_RSHIFT       : return K_KEY_RIGHT_ALT     ;

        case SDLK_BACKSPACE    : return K_KEY_BACKSPACE     ;
        case SDLK_TAB          : return K_KEY_TAB           ;
        case SDLK_RETURN       : return K_KEY_RETURN        ;
        case SDLK_ESCAPE       : return K_KEY_ESCAPE        ;
        case SDLK_SPACE        : return K_KEY_SPACE         ;

        case SDLK_RIGHT        : return K_KEY_RIGHT         ;
        case SDLK_LEFT         : return K_KEY_LEFT          ;
        case SDLK_DOWN         : return K_KEY_DOWN          ;
        case SDLK_UP           : return K_KEY_UP            ;

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

        default: return K_KEY_ENUM_END;
    }
}

void k__refresh_keyboard(void) {

    size_t i = 0;
    for (; i < K_KEY_ENUM_END; i++) {
        key_state[i] = (key_state[i] & 0b11) << 2 | key_state[i] & 0b10000;
    }
}

void k__set_key_down(SDL_Keycode SDL_key_code) {
    enum k_keyboard_key k_key = SDL_key_to_k_key(SDL_key_code);

    key_state[k_key] |= 0b10010;
}

void k__set_key_up(SDL_Keycode SDL_key_code) {
    enum k_keyboard_key k_key = SDL_key_to_k_key(SDL_key_code);

    key_state[k_key] |= 0b01;
    key_state[k_key] &= 0b01111;
}

int k_is_key_pressed(enum k_keyboard_key key) {
    unsigned int n = key_state[key] & 0b1111;

    return n == 0b0010
        || n == 0b0011
        || n == 0b0110
        || n == 0b0111
        || n == 0b1110
        || n == 0b1111;
}

int k_is_key_released(enum k_keyboard_key key) {
    unsigned int n = key_state[key] & 0b1111;

    return n == 0b0001
        || n == 0b0011
        || n == 0b0111
        || n == 0b1001
        || n == 0b1011
        || n == 0b1111;
}

int k_is_key_held(enum k_keyboard_key key) {
    unsigned int n = key_state[key] & 0b1111;

    return n == 0b1000
        || n == 0b1010;
}

int k_is_key_idle(enum k_keyboard_key key) {
    unsigned int n = key_state[key] & 0b1111;

    return n == 0b0000
        || n == 0b0100
        || n == 0b1100;
}
