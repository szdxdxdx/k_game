#ifndef K_ROOM_CREATE_H
#define K_ROOM_CREATE_H

#include "k/list.h"

struct k_room_context;

struct k_room_registry {

    struct k_list iter_list;
};

extern struct k_room_registry * const k_room_registry;

int k_init_room_registry(void);

void k_deinit_room_registry(void);

int k_room_registry_add(struct k_room_context *room);

#endif
