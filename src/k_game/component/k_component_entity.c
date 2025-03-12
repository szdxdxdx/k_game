#include "./k_component.h"

void *k_component_get_data(struct k_component *component) {
    return component->data;
}

struct k_object *k_component_get_object(struct k_component *component) {
    return component->object;
}

struct k_component_manager *k_component_get_manager(struct k_component *component) {
    return component->manager;
}
