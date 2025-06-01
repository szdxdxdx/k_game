#ifndef K_KEYBOARD_H
#define K_KEYBOARD_H

/**
 * \brief 键盘按键的键码
 *
 * 为方便编程，k_game 使用 ASCII 码作为字母键、符号键的键码。
 *
 * 需要修饰符来修改按键的行为才能打出的字符，对应的键码不可用。
 * 例如，k_game 不会直接识别出 `#`，而是识别出 `shift` 和 `3`。
 *
 * `K_KEY_UNUSED_XXX` 一类的键码仅用作占位，不对应任何按键。
 * `K_KEY_ENUM_END` 用来表示最后一个枚举值，不对应任何按键。
 */
enum k_keyboard_key {

    K_KEY_UNUSED_0       ,
    K_KEY_UNUSED_1       ,
    K_KEY_UNUSED_2       ,
    K_KEY_UNUSED_3       ,
    K_KEY_UNUSED_4       ,
    K_KEY_UNUSED_5       ,
    K_KEY_UNUSED_6       ,
    K_KEY_UNUSED_7       ,

    K_KEY_BACKSPACE      = '\b', // 退格键
    K_KEY_TAB            = '\t', // tab 键
    K_KEY_RETURN         = '\n', // 回车键

    K_KEY_CAPSLOCK       , // 大小写锁定键
    K_KEY_DELETE         , // 删除键
    K_KEY_UNUSED_13      ,

    // 左右 ctrl、shift、alt
    K_KEY_LEFT_CTRL      ,
    K_KEY_LEFT_SHIFT     ,
    K_KEY_LEFT_ALT       ,
    K_KEY_RIGHT_CTRL     ,
    K_KEY_RIGHT_SHIFT    ,
    K_KEY_RIGHT_ALT      ,

    K_KEY_UNUSED_20      ,
    K_KEY_UNUSED_21      ,
    K_KEY_UNUSED_22      ,
    K_KEY_UNUSED_23      ,
    K_KEY_UNUSED_24      ,
    K_KEY_UNUSED_25      ,
    K_KEY_UNUSED_26      ,

    K_KEY_ESCAPE         = '\x1B', // ESC 键

    // 方向键
    K_KEY_RIGHT          ,
    K_KEY_LEFT           ,
    K_KEY_DOWN           ,
    K_KEY_UP             ,

    // 主键盘
    K_KEY_SPACE          = ' ',
    K_KEY_EXCLAIM        = '!',
    K_KEY_QUOTE_DBL      = '"',
    K_KEY_HASH           = '#',
    K_KEY_DOLLAR         = '$',
    K_KEY_PERCENT        = '%',
    K_KEY_AMPERSAND      = '&',
    K_KEY_QUOTE          = '\'',
    K_KEY_LEFT_PAREN     = '(',
    K_KEY_RIGHT_PAREN    = ')',
    K_KEY_ASTERISK       = '*',
    K_KEY_PLUS           = '+',
    K_KEY_COMMA          = ',',
    K_KEY_MINUS          = '-',
    K_KEY_PERIOD         = '.',
    K_KEY_SLASH          = '/',
    K_KEY_0              = '0',
    K_KEY_1              = '1',
    K_KEY_2              = '2',
    K_KEY_3              = '3',
    K_KEY_4              = '4',
    K_KEY_5              = '5',
    K_KEY_6              = '6',
    K_KEY_7              = '7',
    K_KEY_8              = '8',
    K_KEY_9              = '9',
    K_KEY_COLON          = ':',
    K_KEY_SEMICOLON      = ';',
    K_KEY_LESS           = '<',
    K_KEY_EQUALS         = '=',
    K_KEY_GREATER        = '>',
    K_KEY_QUESTION       = '?',
    K_KEY_AT             = '@',
    K_KEY_A              = 'A',
    K_KEY_B              = 'B',
    K_KEY_C              = 'C',
    K_KEY_D              = 'D',
    K_KEY_E              = 'E',
    K_KEY_F              = 'F',
    K_KEY_G              = 'G',
    K_KEY_H              = 'H',
    K_KEY_I              = 'I',
    K_KEY_J              = 'J',
    K_KEY_K              = 'K',
    K_KEY_L              = 'L',
    K_KEY_M              = 'M',
    K_KEY_N              = 'N',
    K_KEY_O              = 'O',
    K_KEY_P              = 'P',
    K_KEY_Q              = 'Q',
    K_KEY_R              = 'R',
    K_KEY_S              = 'S',
    K_KEY_T              = 'T',
    K_KEY_U              = 'U',
    K_KEY_V              = 'V',
    K_KEY_W              = 'W',
    K_KEY_X              = 'X',
    K_KEY_Y              = 'Y',
    K_KEY_Z              = 'Z',
    K_KEY_LEFT_BRACKET   = '[',
    K_KEY_BACKSLASH      = '\\',
    K_KEY_RIGHT_BRACKET  = ']',
    K_KEY_CARET          = '^',
    K_KEY_UNDERSCORE     = '_',
    K_KEY_BACK_QUOTE     = '`',

    // 小键盘
    K_KEY_KP_DIVIDE      , // 除号键 /
    K_KEY_KP_MULTIPLY    , // 乘号键 *
    K_KEY_KP_MINUS       , // 减号键 -
    K_KEY_KP_PLUS        , // 加号键 +
    K_KEY_KP_ENTER       , // 回车键
    K_KEY_KP_0           ,
    K_KEY_KP_1           ,
    K_KEY_KP_2           ,
    K_KEY_KP_3           ,
    K_KEY_KP_4           ,
    K_KEY_KP_5           ,
    K_KEY_KP_6           ,
    K_KEY_KP_7           ,
    K_KEY_KP_8           ,
    K_KEY_KP_9           ,
    K_KEY_KP_PERIOD      , // 点号键 .

    // 功能键
    K_KEY_F1             ,
    K_KEY_F2             ,
    K_KEY_F3             ,
    K_KEY_F4             ,
    K_KEY_F5             ,
    K_KEY_F6             ,
    K_KEY_F7             ,
    K_KEY_F8             ,
    K_KEY_F9             ,
    K_KEY_F10            ,
    K_KEY_F11            ,
    K_KEY_F12            ,

    K_KEY_ENUM_END       = K_KEY_F12,
};

/** \brief 检测键盘按键是否在当前帧内被按下 */
int k_key_down(enum k_keyboard_key key);

/** \brief 检测键盘按键是否持续被按住 */
int k_key_held(enum k_keyboard_key key);

/** \brief 检测键盘按键是否在当前帧被按下或按住 */
int k_key_down_or_held(enum k_keyboard_key key);

/** \brief 松开键盘按键是否在当前帧内被松开 */
int k_key_up(enum k_keyboard_key key);

/** \brief 检测键盘按键是否处于空闲状态 */
int k_key_idle(enum k_keyboard_key key);

#endif
