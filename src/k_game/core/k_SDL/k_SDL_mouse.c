#include <stdio.h>

#include "./k_SDL.h"

struct k_mouse_state {
    int x;
    int y;
};

static struct k_mouse_state mouse_state = {
    .x = 0,
    .y = 0,
};

void k__handle_SDL_mouse_motion_event(struct SDL_MouseMotionEvent *event) {

   // printf("Mouse Motion Event:\n");
   // printf("  Window ID: %u\n", event->windowID);
   // printf("  Mouse Instance ID: %u\n", event->which);
   // printf("  Button State: %u\n", event->state);
   // printf("  Position: (%d, %d)\n", event->x, event->y);
   // printf("  Relative Motion: (%d, %d)\n", event->xrel, event->yrel);

    mouse_state.x = event->x;
    mouse_state.y = event->y;
}

int k_mouse_x(void) {
    return mouse_state.x;
}

int k_mouse_y(void) {
    return mouse_state.y;
}
