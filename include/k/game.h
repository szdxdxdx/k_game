#ifndef K_GAME_H
#define K_GAME_H

#include <stddef.h>
#include <stdint.h>

/* region [game] */

struct k_game_config;

int k_run_game(const struct k_game_config *config);

struct k_game_config {

    const char *window_title;

    int window_w;
    int window_h;

    int (*fn_init)(void);
    void (*fn_cleanup)(void);
};

extern const struct k_game_config K_GAME_CONFIG_INIT;

/* endregion */

/* region [room] */

struct k_room_config;
struct k_room;

struct k_room *k_create_room(const struct k_room_config *config);

struct k_room_config {

    const char *room_name;

    size_t data_size;

    int steps_per_second;

    int  (*fn_create) (const struct k_room *room);
    void (*fn_destroy)(const struct k_room *room);
    int  (*fn_enter)  (const struct k_room *room);
    void (*fn_leave)  (const struct k_room *room);
    void (*fn_step)   (const struct k_room *room);
    void (*fn_draw)   (const struct k_room *room);
};

extern const struct k_room_config K_ROOM_CONFIG_INIT;

void k_destroy_room(struct k_room *room);

int k_goto_room(struct k_room *room);

/* endregion */

/* region [image] */

struct k_image;

struct k_image *k_load_image(const char *path);

void k_draw_image(struct k_image *img, int x, int y);

/* endregion */

#endif
