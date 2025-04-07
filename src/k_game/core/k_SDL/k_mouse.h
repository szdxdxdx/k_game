#ifndef K__MOUSE_H
#define K__MOUSE_H

#include "SDL_events.h"

void k__mouse_refresh_state(void);

void k__mouse_update_cursor_position_after_view_change();

void k__SDL_handle_event_mouse_button_down(SDL_MouseButtonEvent *event);

void k__SDL_handle_event_mouse_button_up(SDL_MouseButtonEvent *event);

void k__SDL_handle_event_mouse_motion(SDL_MouseMotionEvent *event);

#endif
