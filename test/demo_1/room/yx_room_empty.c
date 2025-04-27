#include <stdio.h>
#include <stdlib.h>

#include "k_game.h"
#include "k_webui.h"

#include "./yx_room_empty.h"

static int int_val;

static int init(void *data, void *params) {
    *((int **)data) = (int *)params;
    return 0;
}

static int webui_set(void *data, const char *val) {
    int *p = *(int **)data;

    printf("%s", val);

    *p = atoi(val);
    return 0;
}

static void enter_room(void) {

    struct k_webui_binding_config config;
    config.data_size = sizeof(int *);
    config.fn_init   = init;
    config.fn_unbind = NULL;
    config.fn_webui_set = webui_set;


    k_webui_log_info("测试 - %s", "输出一条日志");


    k_webui_bind("tmp", &config, &int_val);


    //k_webui_bind_int("test:a", &int_val);

}

struct k_room *yx_create_empty_room(void) {

    struct k_room_config config = K_ROOM_CONFIG_INIT;
    config.fn_enter = enter_room;

    return k_room_create(&config, NULL);
}
