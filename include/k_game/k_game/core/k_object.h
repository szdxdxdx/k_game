#ifndef K_OBJECT_H
#define K_OBJECT_H

#include <stddef.h>

#include "./k_game_fwd.h"

/* region [object_create] */

struct k_object *k_object_create(size_t data_size);

void k_object_destroy(struct k_object *object);

/* endregion */

/* region [object_get] */

void *k_object_get_data(struct k_object *object);

/* endregion */

/* region [experimental api] */

void k_object_set_destroy_callback(struct k_object *object, void (*fn_callback)(struct k_object *object));

/* endregion */

#endif
