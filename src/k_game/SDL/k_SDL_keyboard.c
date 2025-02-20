#include "./k_SDL_keyboard.h"

static unsigned int k__key_state[K_KEY_ENUM_NUM];

enum k_keyboard_key k__SDL_key_map(SDL_Keycode SDL_key_code) {

    switch (SDL_key_code) {
        case SDLK_RETURN:    return K_KEY_RETURN;
        case SDLK_ESCAPE:    return K_KEY_ESCAPE;
        case SDLK_BACKSPACE: return K_KEY_BACKSPACE;
        case SDLK_TAB:       return K_KEY_TAB;
        case SDLK_SPACE:     return K_KEY_SPACE;

        case SDLK_EXCLAIM:      return K_KEY_EXCLAIM;
        case SDLK_QUOTEDBL:     return K_KEY_QUOTE_DBL;
        case SDLK_HASH:         return K_KEY_HASH;
        case SDLK_PERCENT:      return K_KEY_PERCENT;
        case SDLK_DOLLAR:       return K_KEY_DOLLAR;
        case SDLK_AMPERSAND:    return K_KEY_AMPERSAND;
        case SDLK_QUOTE:        return K_KEY_QUOTE;
        case SDLK_LEFTPAREN:    return K_KEY_LEFT_PAREN;
        case SDLK_RIGHTPAREN:   return K_KEY_RIGHT_PAREN;
        case SDLK_ASTERISK:     return K_KEY_ASTERISK;
        case SDLK_PLUS:         return K_KEY_PLUS;
        case SDLK_COMMA:        return K_KEY_COMMA;
        case SDLK_MINUS:        return K_KEY_MINUS;
        case SDLK_PERIOD:       return K_KEY_PERIOD;
        case SDLK_SLASH:        return K_KEY_SLASH;
        case SDLK_COLON:        return K_KEY_COLON;
        case SDLK_SEMICOLON:    return K_KEY_SEMICOLON;
        case SDLK_LESS:         return K_KEY_LESS;
        case SDLK_EQUALS:       return K_KEY_EQUALS;
        case SDLK_GREATER:      return K_KEY_GREATER;
        case SDLK_QUESTION:     return K_KEY_QUESTION;
        case SDLK_AT:           return K_KEY_AT;
        case SDLK_LEFTBRACKET:  return K_KEY_LEFT_BRACKET;
        case SDLK_BACKSLASH:    return K_KEY_BACKSLASH;
        case SDLK_RIGHTBRACKET: return K_KEY_RIGHT_BRACKET;
        case SDLK_CARET:        return K_KEY_CARET;
        case SDLK_UNDERSCORE:   return K_KEY_UNDERSCORE;
        case SDLK_BACKQUOTE:    return K_KEY_BACK_QUOTE;

        case SDLK_0: return K_KEY_0;
        case SDLK_1: return K_KEY_1;
        case SDLK_2: return K_KEY_2;
        case SDLK_3: return K_KEY_3;
        case SDLK_4: return K_KEY_4;
        case SDLK_5: return K_KEY_5;
        case SDLK_6: return K_KEY_6;
        case SDLK_7: return K_KEY_7;
        case SDLK_8: return K_KEY_8;
        case SDLK_9: return K_KEY_9;

        case SDLK_a: return K_KEY_A;
        case SDLK_b: return K_KEY_B;
        case SDLK_c: return K_KEY_C;
        case SDLK_d: return K_KEY_D;
        case SDLK_e: return K_KEY_E;
        case SDLK_f: return K_KEY_F;
        case SDLK_g: return K_KEY_G;
        case SDLK_h: return K_KEY_H;
        case SDLK_i: return K_KEY_I;
        case SDLK_j: return K_KEY_J;
        case SDLK_k: return K_KEY_K;
        case SDLK_l: return K_KEY_L;
        case SDLK_m: return K_KEY_M;
        case SDLK_n: return K_KEY_N;
        case SDLK_o: return K_KEY_O;
        case SDLK_p: return K_KEY_P;
        case SDLK_q: return K_KEY_Q;
        case SDLK_r: return K_KEY_R;
        case SDLK_s: return K_KEY_S;
        case SDLK_t: return K_KEY_T;
        case SDLK_u: return K_KEY_U;
        case SDLK_v: return K_KEY_V;
        case SDLK_w: return K_KEY_W;
        case SDLK_x: return K_KEY_X;
        case SDLK_y: return K_KEY_Y;
        case SDLK_z: return K_KEY_Z;

        case SDLK_F1:  return K_KEY_F1;
        case SDLK_F2:  return K_KEY_F2;
        case SDLK_F3:  return K_KEY_F3;
        case SDLK_F4:  return K_KEY_F4;
        case SDLK_F5:  return K_KEY_F5;
        case SDLK_F6:  return K_KEY_F6;
        case SDLK_F7:  return K_KEY_F7;
        case SDLK_F8:  return K_KEY_F8;
        case SDLK_F9:  return K_KEY_F9;
        case SDLK_F10: return K_KEY_F10;
        case SDLK_F11: return K_KEY_F11;
        case SDLK_F12: return K_KEY_F12;

        default: return K_KEY_NO_SUPPORT;
    }
}

void k__refresh_keyboard(void) {

    size_t i = 0;
    for (; i < K_KEY_ENUM_NUM; i++)
        k__key_state[i] <<= 1;
}
