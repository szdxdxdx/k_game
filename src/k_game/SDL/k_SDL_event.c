#include <stdio.h>

#include "SDL.h"

#include "../game/k_game_context.h"
#include "../room/k_room_context.h"
#include "./k_SDL_event.h"
#include "k_SDL_keyboard.h"

void k__poll_SDL_events(void) {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

            case SDL_QUIT:
                k__game.quit_game = 1;
                k__game.current_room->game_loop = 0;
                break;

            case SDL_MOUSEMOTION:
                //printf("mouse position = (%d, %d)\n", event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                //printf("mouse button down %d\n", event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                //printf("mouse button up %d\n", event.button.button);
                break;

            case SDL_MOUSEWHEEL:
                //printf("mouse wheel x=%d, y=%d\n", event.wheel.x, event.wheel.y);
                break;

            case SDL_KEYDOWN:
                //printf("key down %s\n", SDL_GetKeyName(event.key.keysym.sym));
                k__set_key_down(event.key.keysym.sym);
                break;

            case SDL_KEYUP:
                k__set_key_up(event.key.keysym.sym);
                break;

            default:
                break;
        }
    }
}
