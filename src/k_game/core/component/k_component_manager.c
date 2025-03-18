#include "k_array.h"

#include "./k_component.h"
#include "../room/k_room.h"

/* region [component_manager_map] */

/* 全局单例容器，用于管理所有的组件管理器实例 */
struct k_room_component_manager_map {

    /* 使用动态二维数组建立从【房间、组件类型】到【组件管理器实例】的索引，
     * 能以 O(1) 的时间复杂度，快速获取特定房间中挂载的特定类型的组件管理器实例。
     *
     * 用类似 C++ 的模板语法来表示 map 的类型应为：
     * k_array<k_array<k_component_manager *> *> map;
     */
    struct k_array map;
};

static struct k_room_component_manager_map component_manager_map;

/* 常量，表示无效数组，类似于 `NULL`
 *
 * 在 `component_manager_map` 关于动态二维数组的实现逻辑中，
 * 第二维度的数组为 `NULL` 等价于 `arr->size == 0`。
 * 使用该常量而不是 `NULL` 可省去额外的 `NULL != arr` 检查。
 */
static struct k_array * const NULL_ARRAY = &(struct k_array){ .size=0 };

int k__component_manager_map_init(void) {

    struct k_array_config config;
    config.fn_malloc     = k_malloc;
    config.fn_free       = k_free;
    config.elem_size     = sizeof(struct k_array *);
    config.init_capacity = 8;
    struct k_array *room_array = k_array_construct(&component_manager_map.map, &config);
    if (NULL == room_array)
        return -1;

    struct k_array **p = k_array_shift_right(room_array, 0, room_array->capacity);
    size_t i = 0;
    for (; i < room_array->size; i++)
        p[i] = NULL_ARRAY;

    return 0;
}

void k__component_manager_map_free(void) {

    struct k_array *room_array = &component_manager_map.map;
    size_t i = 0;
    for (; i < room_array->size; i++) {
        struct k_array *manager_array = k_array_get_elem(room_array, i, struct k_array *);

        /* k_game 销毁所有房间后才清理 `component_manager_map`，
         * 而销毁房间时已移除完该房间的组件管理器，
         * 所以下述 if 判断结果理应为 false。
         */
        if (NULL_ARRAY != manager_array)
            k_array_destroy(manager_array);
    }

    k_array_destruct(room_array);
}

static int k__component_manager_map_add(struct k_room *room, struct k_component_manager *manager) {

    size_t room_id = room->room_id;
    size_t manager_type_id = manager->component_type->manager_type->type_id;

    struct k_array *room_array = &component_manager_map.map;
    struct k_array **p_manager_array;
    struct k_component_manager **p_manager;

    if (room_array->size <= room_id)
        goto room_array_grow;

    p_manager_array = k_array_get_elem_addr(room_array, room_id);
    if (NULL_ARRAY == *p_manager_array) {
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
            *p = NULL_ARRAY;
    }

manager_array_create:
    {
        size_t init_capacity = 8;
        if (init_capacity < manager_type_id)
            init_capacity = manager_type_id;

        struct k_array_config config;
        config.fn_malloc     = k_malloc;
        config.fn_free       = k_free;
        config.elem_size     = sizeof(struct k_component_manager *);
        config.init_capacity = init_capacity;
        *p_manager_array = k_array_create(&config);
        if (NULL == *p_manager_array) {
            *p_manager_array = NULL_ARRAY;
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

static void k__component_manager_map_del(struct k_room *room, struct k_component_manager_type *manager_type) {

    struct k_array *room_array = &component_manager_map.map;
    if (room_array->size <= room->room_id)
        return;

    struct k_array *manager_array = k_array_get_elem(room_array, room->room_id, struct k_array *);
    if (manager_array->size <= manager_type->type_id)
        return;

    struct k_component_manager **p_manager = k_array_get_elem_addr(manager_array, manager_type->type_id);
    *p_manager = NULL;
}

struct k_component_manager *k__component_manager_map_find(struct k_room *room, struct k_component_manager_type *manager_type) {

    struct k_array *room_array = &component_manager_map.map;
    if (room_array->size <= room->room_id)
        return NULL;

    struct k_array *manager_array = k_array_get_elem(room_array, room->room_id, struct k_array *);
    if (manager_array->size <= manager_type->type_id)
        return NULL;

    return k_array_get_elem(manager_array, manager_type->type_id, struct k_component_manager *);
}

/* endregion */

/* region [component_manager_create] */

static int k__component_manager_create(struct k_room *room, struct k_component_type *component_type, void *params) {

    struct k_component_manager_type *manager_type = component_type->manager_type;
    if (NULL == manager_type)
        return -1;

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

    struct k_component_manager *manager = k_malloc(sizeof(struct k_component_manager) + manager_type->data_size);
    if (NULL == manager)
        return -1;

    manager->component_type = component_type;
    manager->room = room;

    if (0 != manager_type->data_size) {
        manager->data = ptr_offset(manager, sizeof(struct k_component_manager));
    } else {
        manager->data = NULL;
    }

    if (0 != k__component_manager_map_add(room, manager))
        goto map_add_failed;

    if (NULL != manager_type->fn_init) {
        if (0 != manager_type->fn_init(manager, params))
            goto fn_init_failed;
    }

    return 0;

fn_init_failed:
    k__component_manager_map_del(room, manager_type);
map_add_failed:
    k_free(manager);
    return -1;
}

static void k__component_manager_destroy_self(struct k_component_manager *manager) {

    struct k_component_manager_type *manager_type = manager->component_type->manager_type;
    if (NULL != manager_type->fn_fini) {
        /* TODO 禁止在 `fn_fini()` 中再次删除自身 */
        manager_type->fn_fini(manager);
    }

    k__component_manager_map_del(manager->room, manager_type);

    k_free(manager);
}

/* endregion */

/* region [room_add_component_manager] */

int k_room_add_component_manager(struct k_room *room, struct k_component_type *component_type, void *params) {
    return k__component_manager_create(room, component_type, params);
}

struct k_component_manager *k_room_get_component_manager(struct k_room *room, struct k_component_type *component_type) {
    return k__component_manager_map_find(room, component_type->manager_type);
}

void k_room_del_component_manager(struct k_room *room, struct k_component_type *component_type) {

    struct k_component_manager *manager = k__component_manager_map_find(room, component_type->manager_type);
    if (NULL == manager)
        return;

    k__component_manager_destroy_self(manager);
}

void k_room_del_all_component_managers(struct k_room *room) {

    struct k_array *room_array = &component_manager_map.map;
    if (room_array->size <= room->room_id)
        return;

    struct k_array **p_manager_array = k_array_get_elem_addr(room_array, room->room_id);
    if (NULL_ARRAY == *p_manager_array)
        return;

    struct k_component_manager **p_manager = k_array_get_elem_addr(*p_manager_array, 0);
    struct k_component_manager **end = k_array_get_elem_addr(*p_manager_array, (*p_manager_array)->size);
    for (; p_manager < end; p_manager++) {

        if (NULL != *p_manager)
            k__component_manager_destroy_self(*p_manager);
    }

    k_array_destroy(*p_manager_array);
    *p_manager_array = NULL_ARRAY;
}

/* endregion */

/* region [component_manager_get] */

void *k_component_manager_get_data(struct k_component_manager *manager) {
    return manager->data;
}

struct k_room *k_component_manager_get_room(struct k_component_manager *manager) {
    return manager->room;
}

/* endregion */
