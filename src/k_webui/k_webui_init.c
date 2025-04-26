#define K_LOG_TAG "k_webui"
#include "k_log.h"

#include "./k_webui_context.h"

struct k_webui_context k__webui;

void k__webui_event_handler(webui_event_t *event) {

    k_log_info("clicked");
}

int k_webui_init(void) {

    k__webui.window = webui_new_window();

    webui_set_size(k__webui.window, 480, 640);
    webui_set_position(k__webui.window, 10, 10);
    webui_set_root_folder(k__webui.window, "./k_webui");

    webui_show(k__webui.window, "./index.html");

    webui_bind(k__webui.window, "k_webui_C_fn_event_handler", k__webui_event_handler);
}

void k_webui_fini(void) {

}
