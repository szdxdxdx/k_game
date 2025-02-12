#ifndef K_GAME_H
#define K_GAME_H

#include <stddef.h>
#include <stdint.h>

struct k_rect {
    int x, y, w, h;
};

struct k_image;
struct k_game_config;
struct k_game_context;
struct k_room_config;
struct k_room;

/* region [game] */

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    const char *window_title;

    int window_w;
    int window_h;

    int (*fn_init)(void);
    void (*fn_cleanup)(void);
};

extern const struct k_game_config K_GAME_CONFIG_INIT;

struct k_game_context {
    struct k_room *current_room;
    float delta_time;
};

extern const struct k_game_context * const k_game;

/* endregion */

/* region [room] */

struct k_room *k_create_room(const struct k_room_config *config);

struct k_room_config {

    const char *room_name;

    size_t data_size;

    int steps_per_second;

    int  (*fn_create) (void);
    void (*fn_destroy)(void);
    int  (*fn_enter)  (void);
    void (*fn_leave)  (void);
    void (*fn_step)   (void);
    void (*fn_draw)   (void);
};

extern const struct k_room_config K_ROOM_CONFIG_INIT;

void k_destroy_room(struct k_room *room);

int k_goto_room(struct k_room *room);

/* endregion */

/* region [image] */

struct k_image *k_load_image(const char *filepath);

int k_put_image(struct k_image *image, int x, int y);

/* endregion */

/* region [sprite] */

struct k_sprite_frame {
    struct k_image *image;
    struct k_rect rect;
    int origin_x;
    int origin_y;
    int delay;
};

struct k_sprite {
    struct k_sprite_frame *frames;
    size_t frames_num;
};

struct k_sprite *k_create_sprite(size_t frames_num);

int k_sprite_set_frame(struct k_sprite *sprite, int frame_index, struct k_sprite_frame *frame);

/* endregion */

#endif
