#include <stddef.h>

#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

/* region [ynx] */

struct k_sprite *yx_spr_ynx_idle;
struct k_sprite *yx_spr_ynx_run;

static int yx__spr_ynx_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/ynx.png";
    config.config_file_path = "./demo_1/sprite/ynx.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_ynx_idle, "idle", 15, 23 },
        { &yx_spr_ynx_run,  "run",  15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [llk] */

struct k_sprite *yx_spr_llk_idle;
struct k_sprite *yx_spr_llk_run;

static int yx__spr_liliko_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/llk.png";
    config.config_file_path = "./demo_1/sprite/llk.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_llk_idle, "idle", 15, 23 },
        { &yx_spr_llk_run,  "run",  15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [bubble] */

struct k_sprite *yx_spr_bubble_appear;
struct k_sprite *yx_spr_bubble_float;
struct k_sprite *yx_spr_bubble_pop;

static int yx__spr_bubble_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/bubble.png";
    config.config_file_path = "./demo_1/sprite/bubble.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_bubble_appear, "appear", 15, 23 },
        { &yx_spr_bubble_float,  "float",  15, 23 },
        { &yx_spr_bubble_pop,    "pop",    15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */


/* region [apple] */

struct k_sprite *yx_spr_weapon_apple;
struct k_sprite *yx_spr_bullet_apple;
struct k_sprite *yx_spr_bullet_apple_crack;

static int yx__spr_weapon_apple_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/weapon_apple.png";
    config.config_file_path = "./demo_1/sprite/weapon_apple.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_weapon_apple,       "apple", 15, 15 },
        { &yx_spr_bullet_apple,       "apple", 15, 15 },
        { &yx_spr_bullet_apple_crack, "crack", 15, 15 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [iris] */

struct k_sprite *yx_spr_iris_gun;
struct k_sprite *yx_spr_iris_bullet;
struct k_sprite *yx_spr_iris_apple;

static int yx__spr_iris_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/iris.png";
    config.config_file_path = "./demo_1/sprite/iris.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_iris_gun,    "gun",    15, 23 },
        { &yx_spr_iris_bullet, "bullet", 16, 16 },
        { &yx_spr_iris_apple,  "apple", 16, 16 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

int yx_spr_load_on_game_start(void) {

    if (0 != yx__spr_ynx_load())
        return -1;
    if (0 != yx__spr_liliko_load())
        return -1;
    if (0 != yx__spr_bubble_load())
        return -1;
    if (0 != yx__spr_weapon_apple_load())
        return -1;
    if (0 != yx__spr_iris_load())
        return -1;

    return 0;
}
