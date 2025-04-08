#ifndef K_CALLBACK_CONTEXT_H
#define K_CALLBACK_CONTEXT_H

struct k_room;
struct k_object;
struct k_component;

void k__room_del_all_callbacks(struct k_room *room);

void k__object_del_all_callbacks(struct k_object *object);

void k__component_del_all_callbacks(struct k_component *component);

#endif
