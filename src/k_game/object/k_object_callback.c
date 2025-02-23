
#include "./k_object_entity.h"

void k__object_init_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    k_list_init(&callbacks_list->list);
}

void k__object_cleanup_callbacks_list(struct k_object *object) {
    struct k_object_callbacks_list *callbacks_list = &object->callbacks_list;

    /* TODO foreach callbacks_list : del callback */
}
