#include "k_log.h"
#include "k_seq_step.h"

#include "k_game.h"

#include "./_internal.h"

/* region [sprite_registry] */

static struct k_asset_registry sprite_registry;

int k__init_sprite_registry(void) {
    return k__asset_registry_init(&sprite_registry);
}

static void release_asset(struct k_asset_registry_node *registry_node) {
    struct k_sprite *sprite = container_of(registry_node, struct k_sprite, registry_node);
    k_sprite_destroy(sprite);
}

void k__cleanup_sprite_registry(void) {
    k__asset_registry_cleanup(&sprite_registry, release_asset);
}

int k_sprite_set_name(struct k_sprite *sprite, const char *sprite_name) {
    return k__asset_set_name(&sprite_registry, &sprite->registry_node, sprite_name);
}

struct k_sprite *k_find_sprite(const char *sprite_name) {

    struct k_asset_registry_node *registry_node = k__asset_registry_find(&sprite_registry, sprite_name);
    if (NULL == registry_node)
        return NULL;

    struct k_sprite *sprite = container_of(registry_node, struct k_sprite, registry_node);
    return sprite;
}

/* endregion */

/* region [sprite_create] */

static int check_config(const struct k_sprite_config *config) {

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 < config->frames_num);

    size_t i = 0;
    for (; i < config->frames_num; i++) {
        check_config_assert(NULL != config->frames[i].image);
        check_config_assert(0 < config->frames[i].delay);
    }

    size_t FRAMES_NUM_MAX = (SIZE_MAX - sizeof(struct k_sprite)) / sizeof(struct k_sprite_frame);
    if (FRAMES_NUM_MAX <= config->frames_num) {
        err_msg = "Too many frames";
        goto err;
    }

    return 0;

err:
    k_log_error("Invalid sprite config: %s", err_msg);
    return -1;
}

struct k_sprite *k_sprite_create(const struct k_sprite_config *config) {

    if (0 != check_config(config))
        goto err;

    /* sprite 结构体和精灵帧 frames 共处同一个大内存块 */
    size_t frames_size = config->frames_num * sizeof(struct k_sprite_frame);
    struct k_sprite *sprite = k_malloc(sizeof(struct k_sprite) + frames_size);
    if (NULL == sprite)
        goto err;

    struct k_sprite_frame *frames = (struct k_sprite_frame *)&(sprite[1]);
    size_t i = 0;
    for (; i < config->frames_num; i++) {
        frames[i].image    = config->frames[i].image;
        frames[i].offset_x = config->frames[i].offset_x;
        frames[i].offset_y = config->frames[i].offset_y;
        frames[i].delay    = config->frames[i].delay;
    }

    sprite->sprite_w   = config->sprite_w;
    sprite->sprite_h   = config->sprite_h;
    sprite->origin_x   = config->origin_x;
    sprite->origin_y   = config->origin_y;
    sprite->frames     = frames;
    sprite->frames_num = config->frames_num;

    k__asset_registry_add(&sprite_registry, &sprite->registry_node);
    return sprite;

err:
    k_log_error("Failed to create sprite");
    return NULL;
}

void k_sprite_destroy(struct k_sprite *sprite) {

    if (NULL == sprite)
        return;

    k__asset_registry_del(&sprite->registry_node);
    k_free(sprite);
}

/* endregion */

/* region [sprite_get] */

int k_sprite_get_width(struct k_sprite *sprite) {
    return sprite->sprite_w;
}

int k_sprite_get_height(struct k_sprite *sprite) {
    return sprite->sprite_h;
}

float k_sprite_get_origin_x(struct k_sprite *sprite) {
    return sprite->origin_x;
}

float k_sprite_get_origin_y(struct k_sprite *sprite) {
    return sprite->origin_y;
}

int k_sprite_get_frames_num(struct k_sprite *sprite) {
    return sprite->frames_num;
}

int k_sprite_get_frame_delay(struct k_sprite *sprite, size_t frame_idx) {
    return sprite->frames[frame_idx].delay;
}

/* endregion */
