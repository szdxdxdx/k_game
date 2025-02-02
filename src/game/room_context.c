#include <limits.h>

#include "k/log.h"
#include "k/list.h"
#include "k/alloc.h"

#include "k/game.h"
#include "k/internal/room_context.h"
#include "k/internal/room_registry.h"

size_t k_create_room(const struct k_room_config *config) {

    struct k_room_context *room = k_malloc(sizeof(*room));
    if (NULL == room)
        goto err;

    if (0 != k_room_registry_add(room))
        goto err;

    return 0;

err:
    return SIZE_MAX;
}
