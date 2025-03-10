#include "k_log.h"
#include "k_seq_step.h"

#include "k_game_alloc.h"

#include "../image/k_image.h"

#include "./k_sprite.h"

/* region [steps] */

struct k_sprite_creation_context {
    const struct k_sprite_config *config;
    struct k_sprite *sprite;
};

static int step_check_config(void *data) {
    struct k_sprite_creation_context *ctx = data;
    const struct k_sprite_config *config = ctx->config;

    const char *err_msg;

#define check_config_assert(cond) \
    do { if ( ! (cond)) { err_msg = "assert( " #cond " )"; goto err; }} while(0)

    check_config_assert(NULL != config);
    check_config_assert(0 != config->frames_num);

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

static int step_malloc_sprite(void *data) {
    struct k_sprite_creation_context *ctx = data;
    const struct k_sprite_config *config = ctx->config;

    size_t frames_size = config->frames_num * sizeof(struct k_sprite_frame);
    struct k_sprite *sprite = k_malloc(sizeof(struct k_sprite) + frames_size);
    if (NULL == sprite)
        return -1;

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

    ctx->sprite = sprite;
    return 0;
}

static void step_free_sprite(void *data) {
    struct k_sprite_creation_context *ctx = data;
    struct k_sprite *sprite = ctx->sprite;

    k_free(sprite);
}

static int step_registry_add(void *data) {
    struct k_sprite_creation_context *ctx = data;
    const struct k_sprite_config *config = ctx->config;

    k__sprite_registry_add(ctx->sprite);
    return 0;
}

static void step_registry_del(void *data) {
    struct k_sprite_creation_context *ctx = data;
    struct k_sprite *sprite = ctx->sprite;

    k__sprite_registry_del(sprite);
}

static const struct k_seq_step steps[] = {
    { step_check_config,  NULL              },
    { step_malloc_sprite, step_free_sprite  },
    { step_registry_add,  step_registry_del },
};

/* endregion */

struct k_sprite *k_sprite_create(const struct k_sprite_config *config) {

    struct k_sprite_creation_context ctx;
    ctx.config = config;
    ctx.sprite = NULL;

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), &ctx)) {
        k_log_error("Failed to create sprite");
        return NULL;
    }

    return ctx.sprite;
}

void k__sprite_destroy(struct k_sprite *sprite) {

    struct k_sprite_creation_context ctx;
    ctx.config = NULL;
    ctx.sprite = sprite;

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}
