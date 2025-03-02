#include "SDL_render.h"

#include "k_log.h"

#include "k_game/k_SDL.h"
#include "k_game_sprite.h"
#include "k_game/sprite_create.h"
#include "k_game/sprite_draw.h"

int k__draw_sprite_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx) {

    struct k_sprite_frame *frame = &sprite->frames[frame_idx];

    SDL_Rect src;
    src.x = frame->offset_x;
    src.y = frame->offset_y;
    src.w = sprite->sprite_w;
    src.h = sprite->sprite_h;

    SDL_Rect dst;
    dst.x = x - sprite->origin_x;
    dst.y = y - sprite->origin_y;
    dst.w = sprite->sprite_w;
    dst.h = sprite->sprite_h;

    if (0 != SDL_RenderCopy(k__window.renderer, frame->texture, &src, &dst)) {
        k_log_error("SDL_RenderCopy() failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

int k_draw_sprite_frame(struct k_sprite *sprite, int x, int y, size_t frame_idx) {
    /* TODO: assert( NULL != sprite ) */
    /* TODO: assert currently is in draw callback */

    return k__draw_sprite_frame(sprite, x, y, frame_idx);
}
