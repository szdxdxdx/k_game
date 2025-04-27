#ifndef K__WEBUI_BIND_H
#define K__WEBUI_BIND_H

#include "k_str_map.h"

int k__webui_binding_init(void);

void k__webui_binding_fini(void);

struct k_webui_binding_manager {

    struct k_str_map group_map;
};

extern struct k_webui_binding_manager k__webui_binding;

struct k_webui_binding_group {

    const char *group_name;

    struct k_str_map label_map;
};

struct k_webui_binding {

    const char *label_name;
};

#endif
