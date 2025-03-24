#include "k_list.h"

#include "k_game.h"

static struct k_position world = {
    .parent = NULL,
    .position.x = 0,
    .position.y = 0,
};

void k_position_init(struct k_position *self, struct k_position *parent, float offset_x, float offset_y) {

    k_list_init(&self->list);

    if (NULL != parent) {
        k_list_add_tail(&parent->list, &self->list_node);
        self->parent = parent;
    } else {
        k_list_node_loop(&self->list_node);
        self->parent = &world;
    }

    self->offset.x = offset_x;
    self->offset.y = offset_y;

    self->position.x = parent->position.x + self->offset.x;
    self->position.y = parent->position.y + self->offset.y;
}

static void update_position(struct k_position *self) {

    self->position.x = self->parent->position.x + self->offset.x;
    self->position.y = self->parent->position.y + self->offset.y;

    struct k_position *child;
    struct k_list *list = &self->list;
    struct k_list_node *iter;
    for (k_list_for_each(list, iter)) {
        child = container_of(iter, struct k_position, list_node);

        update_position(child);
    }
}

void k_position_set_offset(struct k_position *self, float x, float y) {

    self->offset.x = x;
    self->offset.y = y;

    update_position(self);
}
