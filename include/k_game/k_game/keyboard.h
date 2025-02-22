#ifndef K_GAME_KEYBOARD_H
#define K_GAME_KEYBOARD_H

enum k_keyboard_key;

int k_is_key_pressed(enum k_keyboard_key key);

int k_is_key_released(enum k_keyboard_key key);

int k_is_key_held(enum k_keyboard_key key);

int k_is_key_idle(enum k_keyboard_key key);

int k_is_key_down(enum k_keyboard_key key);

int k_is_key_up(enum k_keyboard_key key);

/**
 * \brief
 *
 * `K_KEY__RESERVED_XXX` 表示该键码仅用作占位，不对应任何按键。
 */
enum k_keyboard_key {

    K_KEY__RESERVED_0   ,
    K_KEY__RESERVED_1   ,
    K_KEY__RESERVED_2   ,
    K_KEY__RESERVED_3   ,
    K_KEY__RESERVED_4   ,
    K_KEY__RESERVED_5   ,
    K_KEY__RESERVED_6   ,
    K_KEY__RESERVED_7   ,

    K_KEY_BACKSPACE     = '\b',
    K_KEY_TAB           = '\t',
    K_KEY_RETURN        = '\n',

    K_KEY_CAPSLOCK      ,
    K_KEY_DELETE        ,
    K_KEY_PRINT_SCREEN  ,

    K_KEY__RESERVED_14  ,
    K_KEY__RESERVED_15  ,
    K_KEY__RESERVED_16  ,

    K_KEY_LEFT_CTRL     ,
    K_KEY_LEFT_SHIFT    ,
    K_KEY_LEFT_ALT      ,
    K_KEY_RIGHT_CTRL    ,
    K_KEY_RIGHT_SHIFT   ,
    K_KEY_RIGHT_ALT     ,

    K_KEY__RESERVED_23  ,
    K_KEY__RESERVED_24  ,
    K_KEY__RESERVED_25  ,
    K_KEY__RESERVED_26  ,

    K_KEY_ESCAPE        = '\x1B',

    K_KEY_RIGHT         ,
    K_KEY_LEFT          ,
    K_KEY_DOWN          ,
    K_KEY_UP            ,

    K_KEY_SPACE         = ' ',
    K_KEY_EXCLAIM       = '!',
    K_KEY_QUOTE_DBL     = '"',
    K_KEY_HASH          = '#',
    K_KEY_DOLLAR        = '$',
    K_KEY_PERCENT       = '%',
    K_KEY_AMPERSAND     = '&',
    K_KEY_QUOTE         = '\'',
    K_KEY_LEFT_PAREN    = '(',
    K_KEY_RIGHT_PAREN   = ')',
    K_KEY_ASTERISK      = '*',
    K_KEY_PLUS          = '+',
    K_KEY_COMMA         = ',',
    K_KEY_MINUS         = '-',
    K_KEY_PERIOD        = '.',
    K_KEY_SLASH         = '/',
    K_KEY_0             = '0',
    K_KEY_1             = '1',
    K_KEY_2             = '2',
    K_KEY_3             = '3',
    K_KEY_4             = '4',
    K_KEY_5             = '5',
    K_KEY_6             = '6',
    K_KEY_7             = '7',
    K_KEY_8             = '8',
    K_KEY_9             = '9',
    K_KEY_COLON         = ':',
    K_KEY_SEMICOLON     = ';',
    K_KEY_LESS          = '<',
    K_KEY_EQUALS        = '=',
    K_KEY_GREATER       = '>',
    K_KEY_QUESTION      = '?',
    K_KEY_AT            = '@',
    K_KEY_A             = 'A',
    K_KEY_B             = 'B',
    K_KEY_C             = 'C',
    K_KEY_D             = 'D',
    K_KEY_E             = 'E',
    K_KEY_F             = 'F',
    K_KEY_G             = 'G',
    K_KEY_H             = 'H',
    K_KEY_I             = 'I',
    K_KEY_J             = 'J',
    K_KEY_K             = 'K',
    K_KEY_L             = 'L',
    K_KEY_M             = 'M',
    K_KEY_N             = 'N',
    K_KEY_O             = 'O',
    K_KEY_P             = 'P',
    K_KEY_Q             = 'Q',
    K_KEY_R             = 'R',
    K_KEY_S             = 'S',
    K_KEY_T             = 'T',
    K_KEY_U             = 'U',
    K_KEY_V             = 'V',
    K_KEY_W             = 'W',
    K_KEY_X             = 'X',
    K_KEY_Y             = 'Y',
    K_KEY_Z             = 'Z',
    K_KEY_LEFT_BRACKET  = '[',
    K_KEY_BACKSLASH     = '\\',
    K_KEY_RIGHT_BRACKET = ']',
    K_KEY_CARET         = '^',
    K_KEY_UNDERSCORE    = '_',
    K_KEY_BACK_QUOTE    = '`',

    K_KEY_NUM_LOCK_CLEAR,
    K_KEY_KP_DIVIDE     ,
    K_KEY_KP_MULTIPLY   ,
    K_KEY_KP_MINUS      ,
    K_KEY_KP_PLUS       ,
    K_KEY_KP_ENTER      ,
    K_KEY_KP_0          ,
    K_KEY_KP_1          ,
    K_KEY_KP_2          ,
    K_KEY_KP_3          ,
    K_KEY_KP_4          ,
    K_KEY_KP_5          ,
    K_KEY_KP_6          ,
    K_KEY_KP_7          ,
    K_KEY_KP_8          ,
    K_KEY_KP_9          ,
    K_KEY_KP_PERIOD     ,

    K_KEY_F1            ,
    K_KEY_F2            ,
    K_KEY_F3            ,
    K_KEY_F4            ,
    K_KEY_F5            ,
    K_KEY_F6            ,
    K_KEY_F7            ,
    K_KEY_F8            ,
    K_KEY_F9            ,
    K_KEY_F10           ,
    K_KEY_F11           ,
    K_KEY_F12           ,

    K_KEY_ENUM_END, /* <- 仅表示 `k_keyboard_key` 枚举的个数，不表示任何一个按键 */
};

#endif
