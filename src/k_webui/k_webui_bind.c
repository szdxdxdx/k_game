
#include "./k_webui_context.h"

#define ptr_offset(p, offset) ((void *)((char *)(p) + (offset)))

int k__webui_binding_init(void) {

    struct k_str_map_options options;
    options.fn_malloc = k__webui_mem_alloc;
    options.fn_free   = k__webui_mem_free;

    struct k_str_map *groups = k_str_map_construct(&k__webui.binding_groups, &options);
    if (NULL == groups)
        return -1;

    /* ... */

    return 0;
}

void k__webui_binding_fini(void) {
    /* TODO */
}

static void k__webui_unbind(struct k_webui_binding *binding) {

    if (binding->fn_unbind != NULL) {
        binding->fn_unbind(binding->data);
    }
}

static void k__webui_clear_binding_group_callback(const char *key, void *val) {
    (void)key;

    struct k_webui_binding *binding = val;
    k__webui_unbind(binding);
}

static struct k_webui_binding_group *k__webui_find_or_add_binding_group(const char *group_name) {

    struct k_webui_binding_group *group = k_str_map_get(&k__webui.binding_groups, group_name);
    if (NULL != group)
        return group;

    group = k_str_map_add(&k__webui.binding_groups, group_name, sizeof(struct k_webui_binding_group));
    if (NULL == group)
        return NULL;

    return group;
}

static void k__webui_del_binding_group(struct k_webui_binding_group *group) {

    k_str_map_clear(&group->bindings, k__webui_clear_binding_group_callback);
}

static int k__webui_parse_group_label(char *binding_key, char **get_group, char **get_label) {

    char *p = binding_key;
    char *colon = NULL;
    for (; '\0' != *p; ) {
        if (':' == *p) {
            if (NULL != colon) {
                return -1;
            }
            else {
                colon = p;
                p++;
                if ('\0' == *p)
                    return -1;
            }
        } else {
            p++;
        }
    }

    if (NULL == colon) {
        *get_group = "";
        *get_label = binding_key;
    }
    else {
        *colon = '\0';
        *get_group = binding_key;
        *get_label = colon + 1;
    }

    return 0;
}

int k_webui_bind(const struct k_webui_binding_config *config, void *params) {

    if (NULL == config)
        return -1;
    if (NULL == config->binding_key || '\0' == config->binding_key[0])
        return -1;
    if (NULL == config->fn_webui_set)
        return -1;

    char buf[128];
    strncpy(buf, config->binding_key, sizeof(buf));

    if ('\0' != buf[sizeof(buf) - 1])
        return -1;

    char *group_name;
    char *label_name;
    if (0 != k__webui_parse_group_label(buf, &group_name, &label_name))
        return -1;

    struct k_webui_binding_group *binding_group = k_str_map_get(&k__webui.binding_groups, group_name);
    if (NULL == binding_group)
        return -1;

    struct k_webui_binding *binding = k_str_map_get(&binding_group->bindings, label_name);
    if (NULL != binding)
        return -1;

    binding = k_str_map_add(&binding_group->bindings, label_name, sizeof(struct k_webui_binding) + config->data_size);
    binding->data = ptr_offset(binding, sizeof(struct k_webui_binding));
    binding->fn_unbind = config->fn_unbind;

    if (0 != config->fn_init(binding->data, params)) {
        k_str_map_remove(&binding_group->bindings, label_name);
        return -1;
    }

    return 0;
}

void k_webui_unbind(const char *binding_key) {

    char buf[128];
    strncpy(buf, binding_key, sizeof(buf));

    char *group_name;
    char *label_name;
    if (0 != k__webui_parse_group_label(buf, &group_name, &label_name))
        return;

    struct k_webui_binding_group *binding_group = k_str_map_get(&k__webui.binding_groups, group_name);
    if (NULL == binding_group)
        return;

    struct k_webui_binding *binding = k_str_map_get(&binding_group->bindings, label_name);
    if (NULL == binding)
        return;

    k__webui_unbind(binding);

    k_str_map_remove(&binding_group->bindings, label_name);
}
