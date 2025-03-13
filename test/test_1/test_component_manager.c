#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "k_game.h"

/* region [component_1] */

struct my_component_1 {
    char s[32];
};

struct my_component_1_manager {
    char s[32];
};

static int my_component_1_init(struct k_component *component, void *params) {
    struct k_component_manager *manager = k_component_get_manager(component);

    if (manager == NULL) {
        printf("create component_1, NO manager\n");
    }
    else {
        struct my_component_1_manager *data = k_component_manager_get_data(manager);
        printf("create component_1, manager->s = '%s'\n", data->s);
    }
    return 0;
}

static int my_component_1_manager_init(struct k_component_manager *manager, void *params) {
    struct my_component_1 *data = k_component_manager_get_data(manager);
    sprintf(data->s, "%s", (const char *)params);

    printf("add component_1_manager to %s\n", (const char *)params);
    return 0;
}

static void my_component_1_manager_fini(struct k_component_manager *manager) {
    struct my_component_1 *data = k_component_manager_get_data(manager);
    printf("fini component_1_manager, manager->s = '%s'\n", data->s);
}

static void my_component_1_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct my_component_1);
    entity_config.fn_init = my_component_1_init;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct my_component_1_manager);
    manager_config.fn_init = my_component_1_manager_init;
    manager_config.fn_fini = my_component_1_manager_fini;

    struct k_component_type *component_type = k_component_define(&manager_config, &entity_config);
    k_component_type_set_name(component_type, "my-1");
}

/* endregion */

/* region [component_2] */

struct my_component_2 {
    char s[32];
};

struct my_component_2_manager {
    char s[32];
};

static int my_component_2_init(struct k_component *component, void *params) {
    struct k_component_manager *manager = k_component_get_manager(component);

    if (manager == NULL) {
        printf("create component_2, NO manager\n");
    }
    else {
        struct my_component_2_manager *data = k_component_manager_get_data(manager);
        printf("create component_2, manager->s = '%s'\n", data->s);
    }

    return 0;
}

static int my_component_2_manager_init(struct k_component_manager *manager, void *params) {
    struct my_component_2 *data = k_component_manager_get_data(manager);
    sprintf(data->s, "%s", (const char *)params);

    printf("add component_2_manager to %s\n", (const char *)params);
    return 0;
}

static void my_component_2_manager_fini(struct k_component_manager *manager) {
    struct my_component_2 *data = k_component_manager_get_data(manager);
    printf("fini component_2_manager, manager->s = '%s'\n", data->s);
}

static void my_component_2_define(void) {

    struct k_component_entity_config entity_config = K_COMPONENT_ENTITY_CONFIG_INIT;
    entity_config.data_size = sizeof(struct my_component_2);
    entity_config.fn_init = my_component_2_init;

    struct k_component_manager_config manager_config = K_COMPONENT_MANAGER_CONFIG_INIT;
    manager_config.data_size = sizeof(struct my_component_2_manager);
    manager_config.fn_init = my_component_2_manager_init;
    manager_config.fn_fini = my_component_2_manager_fini;

    struct k_component_type *component_type = k_component_define(&manager_config, &entity_config);
    k_component_type_set_name(component_type, "my-2");
}

/* endregion */

static int room_1_init(void *params) {
    struct k_room *room = k_get_current_room();

    k_room_add_component_manager(room, k_component_type_find("my-1"), "room-1");

    struct k_object *object = k_object_create(0);
    k_object_add_component(object, k_component_type_find("my-1"), NULL);
    k_object_add_component(object, k_component_type_find("my-2"), NULL);

    return 0;
}

static int room_2_init(void *params) {
    struct k_room *room = k_get_current_room();

    struct k_component_type *my_1 = k_component_type_find("my-1");
    struct k_component_type *my_2 = k_component_type_find("my-2");

    k_room_add_component_manager(room, my_1, "room-2");
    k_room_add_component_manager(room, my_2, "room-2");

    struct k_object *object = k_object_create(0);
    k_object_add_component(object, my_1, NULL);
    k_object_add_component(object, my_2, NULL);

    return 0;
}

static int init_game(void) {

    my_component_1_define();
    my_component_2_define();

    struct k_room_config room_1_config = K_ROOM_CONFIG_INIT;
    room_1_config.fn_init = room_1_init;
    struct k_room *room_1 = k_room_create(&room_1_config, NULL);

    struct k_room_config room_2_config = K_ROOM_CONFIG_INIT;
    room_2_config.fn_init = room_2_init;
    struct k_room *room_2 = k_room_create(&room_2_config, NULL);

    k_goto_room(room_1);
    return 0;
}

#if 1

int main(int argc, char **argv) {
    system("chcp 65001");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    struct k_game_config game_config = K_GAME_CONFIG_INIT;
    game_config.window_w = 400;
    game_config.window_h = 360;
    game_config.fn_init = init_game;

    k_game_run(&game_config);

    return 0;
}

#endif
