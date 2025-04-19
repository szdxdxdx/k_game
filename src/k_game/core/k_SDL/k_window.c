#include "./k_window.h"

struct k_window_context k__window;

float k_window_get_width(void) {
    return k__window.window_w;
}

float k_window_get_height(void) {
    return k__window.window_h;
}
