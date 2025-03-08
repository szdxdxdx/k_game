#include "./k_component.h"

struct k_object *k_component_get_object(struct k_component *component) {
    return component->object;
}

void *k_component_get_data(struct k_component *component) {
    return component->data;
}
