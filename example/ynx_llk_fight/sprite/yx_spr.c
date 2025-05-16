#include <stddef.h>

#include "./yx_spr.h"

#include "../utils/sprite_sheet/yx_sprite_sheet.h"

/* region [ynx] */

struct k_sprite *yx_spr_ynx_idle = NULL;
struct k_sprite *yx_spr_ynx_run  = NULL;

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

struct k_sprite *yx_spr_liliko_idle = NULL;
struct k_sprite *yx_spr_liliko_run  = NULL;

static int yx__spr_liliko_load(void) {

    struct yx_sprite_sheet_config config;
    config.image_file_path  = "./demo_1/sprite/liliko.png";
    config.config_file_path = "./demo_1/sprite/liliko.json";
    config.scale = 2.0f;
    config.sprites = (struct yx_sprite_sheet_sprite_config[]){
        { &yx_spr_liliko_idle, "idle", 15, 23 },
        { &yx_spr_liliko_run,  "run",  15, 23 },
        { NULL }
    };

    return yx_sprite_load_from_sheet(&config);
}

/* endregion */

/* region [bubble] */

struct k_sprite *yx_spr_bubble_appear = NULL;
struct k_sprite *yx_spr_bubble_float  = NULL;
struct k_sprite *yx_spr_bubble_pop    = NULL;

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

/* region [iris] */

struct k_sprite *yx_spr_iris_gun    = NULL;
struct k_sprite *yx_spr_iris_bullet = NULL;
struct k_sprite *yx_spr_iris_apple  = NULL;

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

int yx_spr_load(void) {

    /* 若资源加载失败则游戏无法启动，所以此处不需要考虑回滚 */

    if (0 != yx__spr_ynx_load())    return -1;
    if (0 != yx__spr_liliko_load()) return -1;
    if (0 != yx__spr_bubble_load()) return -1;
    if (0 != yx__spr_iris_load())   return -1;

    return 0;
}
