#include "k_log.h"

#include "./k_component_type.h"

static struct k_component_registry component_registry;

void k__component_registry_init(void) {
    k_list_init(&component_registry.components_list);

    static struct k_hash_list buckets[32]; /* <- 或许有必要支持动态扩容？ */
    k_str_map_init(&component_registry.name_map, buckets, 32);
}

void k__component_registry_deinit(void) {

    struct k_component_registry_node *registry_node;
    struct k_component_type *component_type;
    struct k_list_node *iter, *next;
    for(k_list_for_each_s(&component_registry.components_list, iter, next)) {
        registry_node = container_of(iter, struct k_component_registry_node, iter_node);
        component_type = container_of(registry_node, struct k_component_type, registry_node);

        k__component_registry_del(component_type);
    }
}

int k__component_registry_add(struct k_component_type *component_type, const char *type_name) {
    struct k_component_registry_node *registry_node = &component_type->registry_node;

    if (NULL == type_name || '\0' == type_name[0]) {
        registry_node->name_map_node.key = "";
    }
    else {
        if (0 != k_str_map_add(&component_registry.name_map, type_name, &registry_node->name_map_node)) {
            k_log_error("The component type named \"%s\" already exists", type_name);
            return -1;
        }
    }

    k_list_add_tail(&component_registry.components_list, &registry_node->iter_node);
    return 0;
}

void k__component_registry_del(struct k_component_type *component_type) {
    struct k_component_registry_node *registry_node = &component_type->registry_node;

    if ('\0' != registry_node->name_map_node.key[0])
        k_str_map_del(&component_registry.name_map, &registry_node->name_map_node);

    k_list_del(&registry_node->iter_node);
}

struct k_component_type *k_get_component_type_by_name(const char *type_name) {

    if (NULL == type_name || '\0' == type_name[0])
        return NULL;

    struct k_str_map_node *map_node = k_str_map_get(&component_registry.name_map, type_name);
    if (NULL == map_node)
        return NULL;

    struct k_component_registry_node *registry_node = container_of(map_node, struct k_component_registry_node, name_map_node);
    struct k_component_type *component_type = container_of(registry_node, struct k_component_type, registry_node);
    return component_type;
}

const char *k_component_type_get_name(struct k_component_type *component_type) {
    return component_type->registry_node.name_map_node.key;
}
