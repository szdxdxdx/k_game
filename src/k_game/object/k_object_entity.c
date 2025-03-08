#include "./k_object.h"

void *k_object_get_data(struct k_object *object) {
    return object->data;
}
