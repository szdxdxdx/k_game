#define K_LOG_TAG "k_webui"
#include "k_log.h"

#include "./k_webui_internal.h"

struct k_webui_context k__webui = {
    .window = WEBUI_MAX_IDS,
    .is_inited = 0
};

int k_webui_init(void) {

    if (k__webui.is_inited) {
        return -1;
    }

    k__webui.window = webui_new_window();

    webui_set_size(k__webui.window, 500, 640);
    webui_set_position(k__webui.window, 10, 10);
    webui_set_root_folder(k__webui.window, "./k_webui");

    k__webui_binding_init();

    webui_show(k__webui.window, "./index.html");

    k__webui.is_inited = 1;
}

int k__webui_is_running(void) {
    return webui_is_shown(k__webui.window);
}

void k_webui_close(void) {

    if ( ! k__webui.is_inited)
        return;

    k__webui_binding_fini();

    webui_exit();
    webui_clean();

    k__webui.window = WEBUI_MAX_IDS;
    k__webui.is_inited = 0;
}
