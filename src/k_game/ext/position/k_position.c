#include "k_list.h"

#include "k_game.h"

static struct k_position world = {
    .parent = NULL,
    .x = 0,
    .y = 0,
};

void k_position_init(struct k_position *self, struct k_position *parent, float rel_x, float rel_y) {

    k_list_init(&self->list);

    if (NULL != parent) {
        k_list_add_tail(&parent->list, &self->list_node);
        self->parent = parent;
    } else {
        k_list_node_loop(&self->list_node);
        self->parent = &world;
    }

    self->rel_x = rel_x;
    self->rel_y = rel_y;

    self->x = self->parent->x + self->rel_x;
    self->y = self->parent->y + self->rel_y;

    self->data = NULL;
    self->fn_after_move = NULL;
}

static void update_position(struct k_position *self) {

    self->x = self->parent->x + self->rel_x;
    self->y = self->parent->y + self->rel_y;

    struct k_position *child;
    struct k_list *list = &self->list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        child = container_of(iter, struct k_position, list_node);

        update_position(child);
    }

    if (self->fn_after_move != NULL)
        self->fn_after_move(self->data);
}

void k_position_set(struct k_position *self, float rel_x, float rel_y) {

    self->rel_x = rel_x;
    self->rel_y = rel_y;

    update_position(self);
}
