#include "../game/k_game_context.h"
#include "../room/k_room_context.h"

#include "./k_SDL_event.h"
#include "./k_SDL_keyboard.h"

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


            /* TODO: 键位冲突
             * 同时按下多个按键时，SDL 可能无法识别所有按键
             */
            case SDL_KEYDOWN: k__handle_SDL_key_down_event(&event.key); break;
            case SDL_KEYUP:   k__handle_SDL_key_up_event(&event.key);   break;

            default:
                break;
        }
    }
}
