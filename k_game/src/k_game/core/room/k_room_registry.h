#ifndef K__ROOM_REGISTRY_H
#define K__ROOM_REGISTRY_H

struct k_room;

int k__room_registry_init(void);

void k__room_registry_cleanup(void);

void k__room_registry_add(struct k_room *room);

void k__room_registry_del(struct k_room *room);

#endif
