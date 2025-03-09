#include "SDL_image.h"

#include "k_log.h"
#include "k_seq_step.h"

#include "k_game_alloc.h"

#include "../k_SDL/k_SDL.h"

#include "./k_image.h"

/* region [steps] */

struct k_image_load_context {
    const char *filepath;
    const char *image_name;
    struct k_image *image;
};

static int step_check_config(void *data) {
    struct k_image_load_context *ctx = data;

    if (NULL == ctx->filepath || '\0' == ctx->filepath[0])
        return -1;

    return 0;
}

static int step_malloc_image(void *data) {
    struct k_image_load_context *ctx = data;

    struct k_image *image = k_malloc(sizeof(struct k_image));
    if (NULL == image)
        return -1;

    ctx->image = image;
    return 0;
}

static void step_free_image(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    k_free(image);
}

static int step_load_texture(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    image->texture = IMG_LoadTexture(k__window.renderer, ctx->filepath);
    if (NULL == image->texture) {
        k_log_error("IMG_LoadTexture() failed: %s", IMG_GetError());
        return -1;
    }

    return 0;
}

static void step_destroy_texture(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    SDL_DestroyTexture(image->texture);
}

static int step_query_texture(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    if (0 != SDL_QueryTexture(image->texture, NULL, NULL, &image->w, &image->h)) {
        k_log_error("SDL_QueryTexture() failed: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

static int step_registry_add(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    if (0 != k__image_registry_add(image, ctx->image_name))
        return -1;

    return 0;
}

static void step_registry_del(void *data) {
    struct k_image_load_context *ctx = data;
    struct k_image *image = ctx->image;

    k__image_registry_del(image);
}

static const struct k_seq_step steps[] = {
    { step_check_config,  NULL                 },
    { step_malloc_image,  step_free_image      },
    { step_load_texture,  step_destroy_texture },
    { step_query_texture, NULL                 },
    { step_registry_add,  step_registry_del    },
};

/* endregion */

struct k_image *k_image_load(const char *image_name, const char *filepath) {

    struct k_image_load_context ctx;
    ctx.filepath   = filepath;
    ctx.image_name = image_name;
    ctx.image      = NULL;

    if (0 != k_seq_step_exec(steps, k_seq_step_array_len(steps), &ctx)) {
        k_log_error("Failed to load image");
        return NULL;
    }

    return ctx.image;
}

void k__image_release(struct k_image *image) {

    struct k_image_load_context ctx;
    ctx.filepath   = NULL;
    ctx.image_name = NULL;
    ctx.image      = image;

    k_seq_step_exec_backward(steps, k_seq_step_array_len(steps), &ctx);
}
