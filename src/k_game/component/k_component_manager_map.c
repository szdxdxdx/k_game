#include "k_array.h"

#include "k_game_alloc.h"
#include "./k_component.h"

struct k_component_manager_map {

    /* map[room_id][manager_id] = manager */
    struct k_array map;
};

static struct k_component_manager_map manager_map;

static struct k_array ARRAY_NULL = { .size=0, .capacity=0 };

int k__component_manager_map_init(void) {

    size_t init_capacity = 16;

    struct k_array_config config;
    config.fn_malloc     = k_malloc;
    config.fn_free       = k_free;
    config.elem_size     = sizeof(struct k_array *);
    config.init_capacity = init_capacity;

    struct k_array *room_array = k_array_construct(&manager_map.map, &config);
    if (NULL == room_array)
        return -1;

    struct k_array **p = k_array_shift_right(room_array, 0, room_array->capacity);
    size_t i = 0;
    for (; i < room_array->size; i++)
        p[i] = &ARRAY_NULL;

    return 0;
}

void k__component_manager_map_free(void) {

    /* TODO */
}

int k__component_manager_map_add(size_t room_id, size_t manager_type_id, struct k_component_manager *manager) {

    struct k_array *room_array = &manager_map.map;
    struct k_array **p_manager_array;
    struct k_component_manager **p_manager;

    if (room_array->size <= room_id)
        goto room_array_grow;

    p_manager_array = k_array_get_elem_addr(room_array, room_id);
    if (&ARRAY_NULL == *p_manager_array)
        goto manager_array_create;

    if ((*p_manager_array)->size <= manager_type_id)
        goto manager_array_grow;

    p_manager = k_array_get_elem_addr(*p_manager_array, manager_type_id);
    goto manger_array_set_item;

room_array_grow:
    {
        size_t append_size = room_id - room_array->size + 1;
        struct k_array **p = k_array_shift_right(room_array, room_array->size, append_size);
        if (NULL == p)
            return -1;

        p_manager_array = k_array_get_elem_addr(room_array, room_id);

        for (; p < p_manager_array; p++)
            *p = &ARRAY_NULL;
    }

manager_array_create:
    {
        struct k_array_config config;
        config.fn_malloc     = k_malloc;
        config.fn_free       = k_free;
        config.elem_size     = sizeof(struct k_component_manager *);
        config.init_capacity = 0;

        *p_manager_array = k_array_create(&config);
        if (NULL == *p_manager_array) {
            *p_manager_array = &ARRAY_NULL;
            return -1;
        }
    }

manager_array_grow:
    {
        size_t append_size = manager_type_id - (*p_manager_array)->size + 1;
        struct k_component_manager **p = k_array_shift_right(*p_manager_array, 0, append_size);
        if (NULL == p)
            return -1;

        p_manager = k_array_get_elem_addr(*p_manager_array, manager_type_id);

        for (; p < p_manager; p++)
            *p = NULL;
    }

manger_array_set_item:
    *p_manager = manager;
    return 0;
}

void k__component_manager_map_del(size_t room_id, size_t manager_type_id) {

    struct k_array *room_array = &manager_map.map;
    if (room_array->size <= room_id)
        return;

    struct k_array *manager_array = k_array_get_elem(room_array, room_id, struct k_array *);
    if (manager_array->size <= manager_type_id)
        return;

    struct k_component_manager **p_manager = k_array_get_elem_addr(manager_array, manager_type_id);
    *p_manager = NULL;
}

struct k_component_manager *k__component_manager_map_find(size_t room_id, size_t manager_type_id) {

    struct k_array *room_array = &manager_map.map;
    if (room_array->size <= room_id)
        return NULL;

    struct k_array *manager_array = k_array_get_elem(room_array, room_id, struct k_array *);
    if (manager_array->size <= manager_type_id)
        return NULL;

    return k_array_get_elem(manager_array, manager_type_id, struct k_component_manager *);
}
