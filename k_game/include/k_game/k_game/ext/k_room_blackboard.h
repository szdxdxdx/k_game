#ifndef K_ROOM_BLACKBOARD_H
#define K_ROOM_BLACKBOARD_H

#include <stddef.h>

int k_room_add_blackboard(void);

void *k_room_blackboard_add(const char *key, size_t value_size);

void *k_room_blackboard_put(const char *key, size_t value_size);

void *k_room_blackboard_get(const char *key);

void k_room_blackboard_del(const char *key);

#endif
