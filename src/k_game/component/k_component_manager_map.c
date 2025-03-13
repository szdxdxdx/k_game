#include "k_array.h"

#include "k_game_alloc.h"
#include "./k_component.h"

#include "../room/k_room.h"

struct k_component_manager_map {

    /* 使用动态二维数组建立索引：
     * map[room_id][manager_type_id] = manager
     */
    struct k_array map;
};

static struct k_component_manager_map manager_map;

/* 该常量用于表示一个无效的数组，类似于 `NULL`
 *
 * `manager_map` 的部分代码逻辑中 `NULL == arr` 与 `arr->size == 0` 等价，
 * 使用 `NULL_ARRAY` 而非 `NULL`，可以节省一步判断 `NULL != arr`。
 */
static struct k_array NULL_ARRAY = { .size=0, .capacity=0 };

int k__component_manager_map_init(void) {

    struct k_array_config config;
    config.fn_malloc     = k_malloc;
    config.fn_free       = k_free;
    config.elem_size     = sizeof(struct k_array *);
    config.init_capacity = 16;

    struct k_array *room_array = k_array_construct(&manager_map.map, &config);
    if (NULL == room_array)
        return -1;

    struct k_array **p = k_array_shift_right(room_array, 0, room_array->capacity);
    size_t i = 0;
    for (; i < room_array->size; i++)
        p[i] = &NULL_ARRAY;

    return 0;
}

void k__component_manager_map_cleanup_room_all(struct k_room *room) {

}

void k__component_manager_map_free(void) {

    /* TODO */
}

int k__component_manager_map_add(struct k_room *room, struct k_component_manager *manager) {

    size_t room_id = room->room_id;
    size_t manager_type_id = manager->component_type->manager_type->type_id;

    struct k_array *room_array = &manager_map.map;
    struct k_array **p_manager_array;
    struct k_component_manager **p_manager;

    if (room_array->size <= room_id)
        goto room_array_grow;

    p_manager_array = k_array_get_elem_addr(room_array, room_id);
    if (&NULL_ARRAY == *p_manager_array) {
        goto manager_array_create;
    }
    else if ((*p_manager_array)->size <= manager_type_id) {
        goto manager_array_grow;
    }
    else {
        p_manager = k_array_get_elem_addr(*p_manager_array, manager_type_id);
        goto manger_array_set_item;
    }

room_array_grow:
    {
        size_t append_size = room_id - room_array->size + 1;
        struct k_array **p = k_array_shift_right(room_array, room_array->size, append_size);
        if (NULL == p)
            return -1;

        p_manager_array = k_array_get_elem_addr(room_array, room_id);

        for (; p < p_manager_array; p++)
            *p = &NULL_ARRAY;
    }

manager_array_create:
    {
        struct k_array_config config;
        config.fn_malloc     = k_malloc;
        config.fn_free       = k_free;
        config.elem_size     = sizeof(struct k_component_manager *);
        config.init_capacity = 16;

        *p_manager_array = k_array_create(&config);
        if (NULL == *p_manager_array) {
            *p_manager_array = &NULL_ARRAY;
            return -1;
        }
    }

manager_array_grow:
    {
        size_t append_size = manager_type_id - (*p_manager_array)->size + 1;
        struct k_component_manager **p = k_array_shift_right(*p_manager_array, (*p_manager_array)->size, append_size);
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

void k__component_manager_map_del(struct k_room *room, struct k_component_manager_type *manager_type) {

    size_t room_id = room->room_id;
    size_t manager_type_id = manager_type->type_id;

    struct k_array *room_array = &manager_map.map;
    if (room_array->size <= room_id)
        return;

    struct k_array *manager_array = k_array_get_elem(room_array, room_id, struct k_array *);
    if (manager_array->size <= manager_type_id)
        return;

    struct k_component_manager **p_manager = k_array_get_elem_addr(manager_array, manager_type_id);
    *p_manager = NULL;
}

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type) {

    size_t room_id = room->room_id;
    size_t manager_type_id = manager_type->type_id;

    struct k_array *room_array = &manager_map.map;
    if (room_array->size <= room_id)
        return NULL;

    struct k_array *manager_array = k_array_get_elem(room_array, room_id, struct k_array *);
    if (manager_array->size <= manager_type_id)
        return NULL;

    return k_array_get_elem(manager_array, manager_type_id, struct k_component_manager *);
}
