#include <stddef.h>

#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

/* region [ynx] */

struct k_sprite *yx_spr_ynx_idle;
struct k_sprite *yx_spr_ynx_run;
struct k_sprite *yx_spr_ynx_dead;

static int yx__spr_ynx_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/ynx.png";
    config.config_file_path = "./demo_1/sprite/ynx.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_ynx_idle, "idle", 15, 23 },
        { &yx_spr_ynx_run,  "run",  15, 23 },
        { &yx_spr_ynx_dead, "dead", 15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [llk] */

struct k_sprite *yx_spr_llk_idle;
struct k_sprite *yx_spr_llk_run;
struct k_sprite *yx_spr_llk_dead;

static int yx__spr_liliko_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/liliko.png";
    config.config_file_path = "./demo_1/sprite/liliko.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_llk_idle, "idle", 15, 23 },
        { &yx_spr_llk_run,  "run",  15, 23 },
        { &yx_spr_llk_dead, "dead",  15, 23 },
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

/* region [alert_marker] */

struct k_sprite *yx_spr_alert_marker;

static int yx__spr_alert_marker_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/alert_marker.png";
    config.config_file_path = "./demo_1/sprite/alert_marker.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_alert_marker, "alert_marker", 12, 12 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [shadow] */

struct k_sprite *yx_spr_shadow_1;
struct k_sprite *yx_spr_shadow_2;

static int yx__spr_shadow_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/shadow.png";
    config.config_file_path = "./demo_1/sprite/shadow.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_shadow_1, "shadow_1", 15, 15 },
        { &yx_spr_shadow_2, "shadow_2", 15, 15 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [particle_on_hit] */

struct k_sprite *yx_spr_particle_on_hit;

static int yx__spr_particle_on_hit_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/particle_on_hit.png";
    config.config_file_path = "./demo_1/sprite/particle_on_hit.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_particle_on_hit, "particle_on_hit", 15, 15 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [particle_on_create_fighter] */

struct k_sprite *yx_spr_fx_fighter_appear;

static int yx__spr_particle_on_create_fighter_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/particle_on_create_fighter.png";
    config.config_file_path = "./demo_1/sprite/particle_on_create_fighter.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_fx_fighter_appear, "fx_fighter_appear", 16, 80 },
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
        { &yx_spr_iris_apple,  "apple",  16, 16 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [banner_you_win] */

struct k_sprite *yx_spr_banner_you_win;
struct k_sprite *yx_spr_banner_you_died;

static int yx__spr_banner_you_win_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/banner_you_died.png";
    config.config_file_path = "./demo_1/sprite/banner_you_died.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]) {
        { &yx_spr_banner_you_win,  "you_win",  64, 16 },
        { &yx_spr_banner_you_died, "you_died", 64, 16 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

int yx_spr_load_on_game_start(void) {

    return yx__spr_ynx_load()
        || yx__spr_liliko_load()
        || yx__spr_bubble_load()
        || yx__spr_weapon_apple_load()
        || yx__spr_alert_marker_load()
        || yx__spr_shadow_load()
        || yx__spr_particle_on_hit_load()
        || yx__spr_particle_on_create_fighter_load()
        || yx__spr_banner_you_win_load()
    ? -1 : 0;
}
