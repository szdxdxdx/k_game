#ifndef K_GAME_KEYBOARD_H
#define K_GAME_KEYBOARD_H

enum k_keyboard_key;

int k_is_key_pressed(enum k_keyboard_key key);

int k_is_key_held(enum k_keyboard_key key);

int k_is_key_released(enum k_keyboard_key key);

int k_is_key_idle(enum k_keyboard_key key);

enum k_keyboard_key {

    K_KEY_NO_SUPPORT,

    K_KEY_RETURN,        /* 回车 */
    K_KEY_ESCAPE,        /* ESC */
    K_KEY_BACKSPACE,     /* 退格 */
    K_KEY_TAB,           /* tab */
    K_KEY_SPACE,         /* 空格 */
    K_KEY_EXCLAIM,       // !
    K_KEY_QUOTE_DBL,     // "
    K_KEY_HASH,          // #
    K_KEY_PERCENT,       // %
    K_KEY_DOLLAR,        // $
    K_KEY_AMPERSAND,     // &
    K_KEY_QUOTE,         // *
    K_KEY_LEFT_PAREN,    // (
    K_KEY_RIGHT_PAREN,   // )
    K_KEY_ASTERISK,      // *
    K_KEY_PLUS,          // +
    K_KEY_COMMA,         // ,
    K_KEY_MINUS,         // -
    K_KEY_PERIOD,        // .
    K_KEY_SLASH,         // /
    K_KEY_COLON,         // :
    K_KEY_SEMICOLON,     // ;
    K_KEY_LESS,          // <
    K_KEY_EQUALS,        // =
    K_KEY_GREATER,       // >
    K_KEY_QUESTION,      // ?
    K_KEY_AT,            // @
    K_KEY_LEFT_BRACKET,  // [
    K_KEY_BACKSLASH,     // \ 反斜杠
    K_KEY_RIGHT_BRACKET, // ]
    K_KEY_CARET,         // ^
    K_KEY_UNDERSCORE,    // _
    K_KEY_BACK_QUOTE,    // \

    K_KEY_0, K_KEY_1, K_KEY_2, K_KEY_3, K_KEY_4,
    K_KEY_5, K_KEY_6, K_KEY_7, K_KEY_8, K_KEY_9,

    K_KEY_A, K_KEY_B, K_KEY_C, K_KEY_D, K_KEY_E, K_KEY_F, K_KEY_G,
    K_KEY_H, K_KEY_I, K_KEY_J, K_KEY_K, K_KEY_L, K_KEY_M, K_KEY_N,
    K_KEY_O, K_KEY_P, K_KEY_Q, K_KEY_R, K_KEY_S, K_KEY_T,
    K_KEY_U, K_KEY_V, K_KEY_W, K_KEY_X, K_KEY_Y, K_KEY_Z,

    K_KEY_F1, K_KEY_F2, K_KEY_F3, K_KEY_F4,  K_KEY_F5,  K_KEY_F6,
    K_KEY_F7, K_KEY_F8, K_KEY_F9, K_KEY_F10, K_KEY_F11, K_KEY_F12,

    K_KEY_ENUM_NUM,
};

#endif
