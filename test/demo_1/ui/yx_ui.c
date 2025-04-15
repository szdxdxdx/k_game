#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "k_list.h"
#include "k_mem_pool.h"

#include "k_game/core/k_object.h"
#include "k_game/core/k_canvas.h"

/* region [utils] */

int parse_hex_color(const char *str, uint32_t *out_color) {
    if (!str || str[0] != '#' || strlen(str) != 9) return 0;

    uint32_t value = 0;
    int i = 1;
    for (; i < 9; ++i) {
        char c = str[i];
        if (!isxdigit(c)) return 0;

        value <<= 4;
        if (c >= '0' && c <= '9')
            value |= (c - '0');
        else if (c >= 'a' && c <= 'f')
            value |= (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            value |= (c - 'A' + 10);
    }

    *out_color = value;
    return 1;
}
/* endregion */

/* region [ui_context] */

struct yx_ui {

    struct k_object *object;

    struct yx_ui_elem *body;

    struct k_mem_pool mem_pool;
};

void *yx__ui_mem_alloc(struct yx_ui *ui, size_t size) {
    return k_mem_pool_alloc(&ui->mem_pool, size);
}

void yx__ui_mem_free(struct yx_ui *ui, void *p) {
    k_mem_pool_free(&ui->mem_pool, p);
}

struct yx_ui_elem *yx_ui_elem_body_create(struct yx_ui *ui);

struct yx_ui *yx_ui_create_context(void) {

    struct k_object *object = NULL;
    struct k_mem_pool *pool = NULL;
    struct yx_ui_elem *body = NULL;

    object = k_object_create(sizeof(struct yx_ui));
    if (NULL == object)
        goto err;

    struct yx_ui *ui = k_object_get_data(object);

    ui->object = object;

    {
        struct k_mem_pool_config pool_conf;
        pool_conf.fn_malloc        = malloc;
        pool_conf.fn_free          = free;
        pool_conf.alloc_size_align = 8;
        pool_conf.block_size_max   = 512;
        pool_conf.alloc_chunk_size = 4096;
        pool = k_mem_pool_construct(&ui->mem_pool, &pool_conf);
        if (NULL == pool)
            goto err;
    }

    {
        body = yx_ui_elem_body_create(ui);
        if (NULL == body)
            goto err;

        ui->body = body;
    }

    return ui;

err:
    if (NULL != object)
        k_object_destroy(object);
    if (NULL == pool)
        k_mem_pool_destruct(&ui->mem_pool);

    return NULL;
}

void yx_ui_destroy_context(struct yx_ui *ui) {
    /* TODO */
}

struct yx_ui_elem *yx_ui_get_body(struct yx_ui *ui) {
    return ui->body;
}

/* endregion */

/* region [ui_elem] */

struct yx_ui_elem {

    struct k_list_node sibling_node;

    struct k_list child_list;

    struct yx_ui *ui_context;

    void *data;

    void (*fn_draw)(struct yx_ui_elem *elem);

    int (*fn_set_attr)(struct yx_ui_elem *elem, const char *key, const char *val);
};

int yx_ui_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    return elem->fn_set_attr(elem, key, val);
}

int yx_ui_append_child(struct yx_ui_elem *elem, struct yx_ui_elem *child) {

    if (NULL == elem || NULL == child)
        return -1;
    if (elem->ui_context != child->ui_context)
        return -1;

    k_list_add_tail(&elem->child_list, &child->sibling_node);
    return 0;
}

/* region [create_elem] */

static void yx__ui_elem_default_fn_draw(struct yx_ui_elem *elem) {
    (void)elem;
}

static int yx__ui_elem_default_fn_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    (void)elem;
    (void)key;
    (void)val;
    return -1;
}

struct yx_ui_elem *yx__ui_elem_create(struct yx_ui *ui, size_t data_size) {

    struct yx_ui_elem *elem = yx__ui_mem_alloc(ui, sizeof(struct yx_ui_elem));
    if (NULL == elem)
        return NULL;

    void *data = NULL;

    if (0 < data_size) {
        data = yx__ui_mem_alloc(ui, data_size);
        if (NULL == data) {
            yx__ui_mem_free(ui, elem);
            return NULL;
        }
    }

    elem->sibling_node.prev = NULL;
    elem->sibling_node.next = NULL;

    k_list_init(&elem->child_list);

    elem->ui_context = ui;

    elem->data = data;

    elem->fn_draw     = yx__ui_elem_default_fn_draw;
    elem->fn_set_attr = yx__ui_elem_default_fn_set_attr;

    return elem;
}

/* endregion */

/* region [elem_body] */

struct yx_ui_elem *yx_ui_elem_body_create(struct yx_ui *ui) {

    struct yx_ui_elem *elem = yx__ui_elem_create(ui, 0);
    if (NULL == elem)
        return NULL;

    return elem;
}

/* endregion */

/* region [elem_button] */

struct yx_ui_elem_button {
    struct yx_ui_elem *elem;

    float x;
    float y;
    float w;
    float h;
    uint32_t background_color;
};

static int yx_ui_elem_button_set_attr(struct yx_ui_elem *elem, const char *key, const char *val) {
    struct yx_ui_elem_button *button = elem->data;

    if (0 == strncmp(key, "x", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->x = val_f;
    }
    else if (0 == strncmp(key, "y", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->y = val_f;
    }
    else if (0 == strncmp(key, "w", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->w = val_f;
    }
    else if (0 == strncmp(key, "h", 1)) {
        char *end;
        float val_f = strtof(val, &end);
        if (end == val)
            return -1;

        button->h = val_f;
    }
    else if (0 == strncmp(key, "background_color", 16)) {

        uint32_t color;
        if ( ! parse_hex_color(val, &color))
            return -1;

        button->background_color = color;
        return 0;
    }

    return -1;
}

static void yx_ui_elem_button_draw(struct yx_ui_elem *elem) {
    struct yx_ui_elem_button *button = elem->data;

    k_canvas_set_draw_color_rgba(button->background_color);
    k_canvas_fill_rect(button->x, button->y, button->w, button->h);
}

struct yx_ui_elem *yx_ui_elem_button_create(struct yx_ui *ui) {

    struct yx_ui_elem *elem = yx__ui_elem_create(ui, sizeof(struct yx_ui_elem_button));
    if (NULL == elem)
        return NULL;

    struct yx_ui_elem_button *button = elem->data;
    button->elem = elem;
    button->x = 10.0f;
    button->y = 10.0f;
    button->w = 100.0f;
    button->h = 50.0f;
    button->background_color = 0xff0099ff;

    elem->fn_draw = yx_ui_elem_button_draw;
    elem->fn_set_attr = yx_ui_elem_button_set_attr;

    return elem;
}

/* endregion */

struct yx_ui_elem *yx_ui_create_elem(struct yx_ui *ui, const char *elem_type) {

    if (0 == strncmp(elem_type, "button", 6)) {
        return yx_ui_elem_button_create(ui);
    }

    return NULL;
}

/* endregion */

/* region [draw] */

void yx_ui_draw_elem(struct yx_ui_elem *elem) {

    if (elem->fn_draw != NULL)
        elem->fn_draw(elem);

    struct yx_ui_elem *child;
    struct k_list *child_list = &elem->child_list;
    struct k_list_node *iter;
    for (k_list_for_each(child_list, iter)) {
        child = container_of(iter, struct yx_ui_elem, sibling_node);

        yx_ui_draw_elem(child);
    }
}

void yx_ui_draw(struct yx_ui *ui) {
    yx_ui_draw_elem(ui->body);
}

/* endregion */
